#include "opencv2/opencv.hpp"
#include <iostream>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <SFML/Audio.hpp>

#include "src/utility/youtube.h"
#include "src/utility/colors.h"
#include "src/utility/cmd.h"

#include "src/modules/audiorw/audiorw.h"

#include "structs.h"

// Finding terminal size and other Windows specifics
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#define PATH_MAX MAX_PATH
#define sleep(x) Sleep(x * 1000)
#define clear() system("cls")
#else
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO
#define clear() system("clear && printf '\e[3J'")
#endif

// Definitions
#define MAX_SYNC_OFFSET std::chrono::seconds(10) // Do not attempt to A/V sync if error is too large

// Constants
const std::string DENSITY[] = {
    " ", " ", ".", ":", "!", "+", "*", "e", "$", "@", "8",
    ".", "*", "e", "s", "◍",
    "░", "▒", "▓", "█"
};

// Globals
#if !defined(_WIN64) && !defined(_WIN32)
struct winsize termSize;
#endif
std::vector<std::string> buffer;
bool haltLoop = false;
uint16_t scn_col = 80;
uint16_t scn_row = 45;
uint32_t skippedFrames = 0;


// Renderer function, must be used in a separate thread
void decToAscii(cv::VideoCapture cap)
{
    while (!haltLoop) {
        cv::Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty()) break;

        // Resize frame to the size of the terminal
        resize(frame, frame, cv::Size(scn_col, scn_row), 0, 0, cv::INTER_AREA);

        auto* pixelPtr = (uint8_t*)frame.data;
        int cn = frame.channels();
        cv::Scalar_<uint8_t> bgrPixel;

        if (haltLoop) return;
        buffer.emplace_back("");
        buffer[buffer.size() - 1] += "\u001b[" + std::to_string(scn_col) + "D\u001b[" + std::to_string(scn_row) + "A";
        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {
                bgrPixel.val[0] = pixelPtr[i * frame.cols * cn + j * cn + 2]; // R
                bgrPixel.val[1] = pixelPtr[i * frame.cols * cn + j * cn + 1]; // G
                bgrPixel.val[2] = pixelPtr[i * frame.cols * cn + j * cn + 0]; // B

                const auto intensity = uint8_t((bgrPixel[0] + bgrPixel[1] + bgrPixel[2]) / 40.26);
                buffer[buffer.size()-1] += "\u001b[38;5;" + std::to_string(getColorId(
                    bgrPixel[0],
                    bgrPixel[1],
                    bgrPixel[2]
                )) + "m" + DENSITY[intensity];
            }
            if (i != frame.rows - 1) buffer[buffer.size()-1] += "\n";
        }
        buffer.shrink_to_fit();
    }
}

void cleanUp(int signum, bool clear = false)
{
    haltLoop = true;

#if !defined(_WIN64) && !defined(_WIN32)
    system("tput cvvis"); // Restore cursor
#endif

    if (clear) clear(); // Clear terminal

    std::cout << "\u001b[0m"; // Reset color

    if (skippedFrames != 0) writeMsg("Skipped " + std::to_string(skippedFrames) + " frame(s)", LOG_WARN);
    if (signum != 0) writeMsg("Terminating with exit code " + std::to_string(signum), LOG_ERROR);

    exit(signum);
}


void sigIntHandler(int signum)
{
    haltLoop = true;
    clear();
    writeMsg("Received signal " + std::to_string(signum) + ", halting", LOG_WARN);
    cleanUp(signum, true);
}

// Argument actions
FlagActions fActs[] = {
    {"help", "h", true, writeHelp},
    {"github", "g", true, writeGitHub},
    {"info", "i", true, writeBanner},
    {"width", "w", false, nullptr},
    {"height", "h", false, nullptr}
};

// Main function
int main(int argc, char** argv)
{
    // Fast IO speed
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

#if defined(_WIN64) || defined(_WIN32)
    system("chcp 65001 >> NUL"); // Enable cmd.exe unicode mode
#endif

    signal(SIGINT, sigIntHandler); // Register SIGINT signal handler

    // Get terminal size
#if defined(_WIN64) || defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    scn_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    scn_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize); // This only works on Unix
    scn_col = termSize.ws_col;
    scn_row = termSize.ws_row;
#endif

    // Parse command line flags
    const std::vector<FlagOps> cmdFlags = parseArgs(argc, argv, fActs, 3);

    // Handle option flags
    for (const auto & f : cmdFlags) {
        if (f.flag == "width") scn_col = std::stoi(f.val);
        else if (f.flag == "height") scn_row = std::stoi(f.val);
    }

    if (argc < 2) {
        writeMsg("No video path or url provided. Run with the -h flag to view help", LOG_FATAL);
        cleanUp(-1);
    }

    // Hide cursor
#if defined(_WIN64) || defined(_WIN32)
    HANDLE hStdOut;
    CONSOLE_CURSOR_INFO cursorInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &cursorInfo);
#else
    system("tput civis");
#endif

    const std::string urls[3] = {
        "https://youtube.com",
        "https://youtu.be",
        "https://www.youtube.com"
    };

    for (const auto& url : urls) {
        if (strncmp(argv[1], url.c_str(), url.length()) == 0) {
            downloadYouTubeVideo(argv[1]);
            argv[argc - 1] = (char*)"video.mp4";
        }
    }

    // Retrieve absolute path from relative path
    char pathBuff[PATH_MAX];
#if defined(_WIN64) || defined(_WIN32)
    _fullpath(pathBuff, argv[argc - 1], PATH_MAX);
    char* absPath = pathBuff;
#else
    char* absPath = realpath(argv[argc - 1], pathBuff);
#endif

    // MARK: Fork parent process
#if !defined(_WIN64) && !defined(_WIN32)
    pid_t pid = fork();

    if (pid == -1) {
        writeMsg("Failed to fork parent process", LOG_FATAL);
        cleanUp(123);
    }
    // Parent thread
    else if (pid > 0) {
#endif

    cv::VideoCapture capture(absPath);

    // Check if camera was opened successfully
    if (!capture.isOpened()) {
        writeMsg("Error opening video file, check if the file exists and is readable.", LOG_FATAL);
        cleanUp(-1);
    }

    // Calculate target time spent on one frame
    const auto targetDelay = std::chrono::milliseconds(1000) / capture.get(cv::CAP_PROP_FPS);

    std::thread decodeThread(decToAscii, capture); // Start renderer thread

    //  Write banner and loading text to console
    writeBanner();
    writeMsg("Performing initial buffering, please wait a second...", LOG_INFO);
    sleep(1);

    double sample_rate;
    std::vector<std::vector<double>> audio = audiorw::read(absPath, sample_rate);

    // Convert audio data to Int16
    std::vector<sf::Int16> int16Audio;
    for (const auto& channel : audio) {
        for (const auto& sample : channel) {
            int16Audio.push_back(static_cast<sf::Int16>(sample * 32767.0)); // Scale to Int16 range
        }
    }

    sf::SoundBuffer soundBuffer;
    soundBuffer.loadFromSamples(
        int16Audio.data(),
        int16Audio.size(),
        2,
        static_cast<unsigned int>(sample_rate) / 2
    );

    sf::Sound sound;
    sound.setBuffer(soundBuffer);
    sound.play();

    unsigned int i = 0;
    const auto time1 = std::chrono::high_resolution_clock::now();

    clear(); // Clear terminal

    // Main display loop
    while (!haltLoop) {
        if (i >= buffer.size()) {
            break;
        }
        fputs(buffer[i].c_str(), stdout);

        i++;

        const auto time2 = std::chrono::high_resolution_clock::now();
        const auto dur = (targetDelay * (i + 1)) - (time2 - time1);

        if (dur >= std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(dur);

        } else if ((-dur) >= MAX_SYNC_OFFSET) {
            writeMsg("A/V sync error: error is too large, not syncing", LOG_FATAL);
            cleanUp(256);

        } else if ((-dur) > targetDelay) {
            const auto skipFrames = uint8_t((-dur) / targetDelay);
            i += skipFrames;
            skippedFrames += skipFrames;
        }
    }

    decodeThread.detach(); // If the thread is still running, detach it (at this point it should not be running)
    capture.release(); // Release the video capture object

    cleanUp(0, true);

    writeMsg("End of video, thanks for watching!", LOG_INFO);

#if !defined(_WIN64) && !defined(_WIN32)
    }
    // Child thread
    else {
        sleep(1); // Ensure playback starts at the same time

        string path(absPath);
        char *args[] = {
            (char*)"ffplay",
            (char*)"-vn",
            (char*)"-nodisp",
            (char*)path.c_str(),
            NULL
        };

        // Redirect stderr to /dev/null
        const int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);
        dup2(fd, 2); // Change stderr to opened file
        dup2(fd, 1);
        #if defined(_WIN64) || defined(_WIN32)
        system((string("C:\ffmpeg\bin\ffplay\ffplay.exe -vn -nodisp ") + string(absPath)).c_str());
        #else
        execve("/usr/local/bin/ffplay", args, {});
        #endif
        close(fd); // Close file (although this will never happen)
        _exit(EXIT_FAILURE);   // exec never returns
    }
#endif
    return 0;
}
