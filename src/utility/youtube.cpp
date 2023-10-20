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

std::string getVideoDownloadUrl(const std::string& videoUrl)
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
        if (i < progress / 2) {
            std::cout << "█";
        } else if (i == progress / 2) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << progress << "%";
    std::cout.flush();

    return true;
}

void downloadVideo(std::string & url)
{
    try {
        std::regex urlRegex("(https?://[^/]+)(/.*)");

        std::smatch match;
        std::regex_match(url, match, urlRegex);

        std::string baseUrl = match[1];
        std::string path = match[2];

        writeMsg( "Downloading YouTube video...", LOG_INFO);

        std::cout << std::endl;

        httplib::Client client(baseUrl);
        auto response = client.Get(path, downloadCallback);

        std::cout << std::endl << std::endl;

        if (response && response->status == 200) {
            // Open a file to write the video content
            std::ofstream videoFile("video.mp4", std::ios::binary);

            // Write the content to the file
            videoFile.write(response->body.c_str(), long(response->body.length()));

            videoFile.close();
        } else {
            writeMsg("Failed to download video. Please download it manually using the link: " + url, LOG_FATAL);
            exit(1);
        }
    } catch (const std::exception& error) {
        const std::string message = "Error while downloading the video: " + std::string(error.what());
        writeMsg( message, LOG_FATAL);
        exit(1);
    }
}