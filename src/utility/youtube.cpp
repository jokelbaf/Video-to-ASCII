#define CPPHTTPLIB_OPENSSL_SUPPORT // For HTTPS support from httplib
#include "youtube.h"

std::string youtubeUrlToId(const std::string& url) {
    std::regex pattern("(?:v=|\\/)([0-9A-Za-z_-]{11}).*");
    std::smatch matches;

    if (std::regex_search(url, matches, pattern) && matches.size() > 1) {
        return matches[1].str();
    }
    return "";
}

std::string getVideoDownloadUrl(const std::string & videoUrl)
{
    try {
        std::string videoId = youtubeUrlToId(videoUrl);
        if (videoId.empty()) return videoId;

        httplib::Client cli("https://www.youtube.com");

        httplib::Headers headers = {
            {"Content-Type", "application/json"},
            {"User-Agent", "com.google.android.youtube/"}
        };

        std::string jsonBody = R"({
            "contentCheckOk": true,
            "racyCheckOk": true,
            "videoId": ")" + videoId + R"(",
            "context": {
                "client": {
                    "clientName": "ANDROID",
                    "clientVersion": "17.31.35",
                    "androidSdkVersion": 30
                }
            }
        })";

        auto res = cli.Post("/youtubei/v1/player", headers, jsonBody, "application/json");

        if (res && res->status == 200) {
            auto response_json = nlohmann::json::parse(res->body);
            auto formats = response_json["streamingData"]["formats"];
            auto max_format = *std::max_element(
                formats.begin(), formats.end(),
                [](const auto& lhs, const auto& rhs) {
                    return lhs["width"].template get<int>() * lhs["height"].template get<int>() <
                           rhs["width"].template get<int>() * rhs["height"].template get<int>();
            });
            return max_format["url"];
        } else {
            writeMsg("Request to YouTube API filed: " + to_string(res.error()), LOG_FATAL);
            exit(1);
        }
    } catch (const std::exception& error) {
        const std::string message = "Error while downloading the video: " + std::string(error.what());
        writeMsg( message, LOG_FATAL);
        exit(1);
    }
}

bool downloadCallback(uint64_t current, uint64_t total) {
    // Calculate the progress percentage
    int progress = static_cast<int>((total > 0) ? (current * 100 / total) : 0);

    // Print a simple progress bar
    std::cout << "\r[";
    for (int i = 0; i < 50; ++i) {
        if (i < progress / 2 || i == progress / 2) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << progress << "%";
    std::cout.flush();

    return true;
}

bool downloadVideo(std::string & downloadUrl)
{
    try {
        std::regex urlRegex("(https?://[^/]+)(/.*)");

        std::smatch match;
        std::regex_match(downloadUrl, match, urlRegex);

        std::string baseUrl = match[1];
        std::string path = match[2];

        std::cout << std::endl;

        httplib::Client client(baseUrl);
        auto response = client.Get(path, downloadCallback);

        if (response && response->status == 200) {
            // Open a file to write the video content
            std::ofstream videoFile("video.mp4", std::ios::binary);

            // Write the content to the file
            videoFile.write(response->body.c_str(), long(response->body.length()));
            videoFile.close();

            return true;
        }
        return false;

    } catch (const std::exception& error) {
        return false;
    }
}

void downloadYouTubeVideo(const std::string & videoUrl, int maxRetries)
{
    if (youtubeUrlToId(videoUrl).empty()) {
        writeMsg("Invalid YouTube video URL (Unable to get videoId).", LOG_FATAL);
        exit(1);
    }

    writeMsg( "Downloading YouTube video...", LOG_INFO);

    int retries = 0;

    while (retries < maxRetries) {
        try {
            std::cout << std::endl;

            auto url = getVideoDownloadUrl(videoUrl);
            if (downloadVideo(url)) {
                std::cout << std::endl << std::endl;
                return;
            }

            writeMsg("Failed to download the video. Retrying... ", LOG_ERROR);
            retries++;

        } catch (const std::exception& error) {
            retries++;
            writeMsg("Failed to download the video. Retrying... ", LOG_ERROR);
        }
    }
    writeMsg("Unable to download YouTube video with the url provided.", LOG_FATAL);
    exit(0);
}