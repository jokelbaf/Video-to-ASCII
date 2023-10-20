#include "cmd.h"

void writeMsg(const std::string & msg, uint8_t level)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    // Format time
    std::tm tm_now = *std::localtime(&now_c);
    std::cout << GREY_COL << std::put_time(&tm_now, "[%H:%M:%S %d/%m/%Y] ");
    
    switch (level) {
        case LOG_DEBUG:
            std::cout << DEBUG_COL << DEBUG_STR;
            break;
        case LOG_INFO:
            std::cout << INFO_COL << INFO_STR;
            break;
        case LOG_WARN:
            std::cout << WARN_COL << WARN_STR;
            break;
        case LOG_ERROR:
            std::cout << ERROR_COL << ERROR_STR;
            break;
        case LOG_FATAL:
            std::cout << FATAL_COL << FATAL_STR;
            break;
        default:
            std::cout << "[" << INFO_COL << INFO_STR << GREY_COL << "]";
    }

    std::cout << "\u001b[0m" << ": " << msg << std::endl;
}

void writeBanner()
{
    puts("╔════════════════════════════════════════════════════╗");
    puts("║              C++ ASCII Video Player                ║");
    puts("╠════════════════════════════════════════════════════╣");
    puts(std::string(std::string("║  Version ") + VERSION_STR + ", compiled " + __TIMESTAMP__ + "  ║").c_str());
    puts("║                                                    ║");
    puts("║  Originally written by Vincent Kwok and Wang Zerui ║");
    puts("║                                                    ║");
    puts("║  Improved by JokelBaf:                             ║");
    puts("║   • Added YouTube videos support.                  ║");
    puts("║   • Added video sound support.                     ║");
    puts("║   • Optimized code and fixed known issues.         ║");
    puts("║                                                    ║");
    puts("║  Plays a video from a file or YouTube url in your  ║");
    puts("║  terminal.                                         ║");
    puts("╚════════════════════════════════════════════════════╝");
}

void writeHelp()
{
    puts("╔═════════════════════════════════════════════════════════════════════╗");
    puts("║                     C++ ASCII Video Player Help                     ║");
    puts("╠═════════════════════════════════════════════════════════════════════╣");
    puts("║                              Commands                               ║");
    puts("╠═════════╦═══════════════════════════════════════════════════════════╣");
    puts("║ Command ║                        Description                        ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    puts("║ help    ║ View info about all commands. You're here right meow.     ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    puts("║ github  ║ See links to the source code.                             ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    puts("║ info    ║ Information about this program, developers etc.           ║");
    puts("╠═════════╩═══════════════════════════════════════════════════════════╣");
    puts("║                               Flags                                 ║");
    puts("╠═════════╦═══════════════════════════════════════════════════════════╣");
    puts("║ Flag    ║                        Description                        ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    puts("║ width   ║ Override width of viewport (in characters).               ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    puts("║ height  ║ Override height of viewport (in characters).              ║");
    puts("╚═════════╩═══════════════════════════════════════════════════════════╝");
}

void writeGitHub()
{
    puts("╔═════════════════════════════════════════════════════════════════════╗");
    puts("║                C++ ASCII Video Player Source Code                   ║");
    puts("╠═════════════════════════════════════════════════════════════════════╣");
    puts("║ Original repo: https://github.com/theriseof9/ascii-video-player     ║");
    puts("║                                                                     ║");
    puts("║ Improved repo: https://github.com/jokelbaf/ascii-video-player       ║");
    puts("╚═════════════════════════════════════════════════════════════════════╝");
}

std::vector<FlagOps> parseArgs(int argc, char** argv, FlagActions fActions[], uint16_t fArgs)
{
    std::vector<FlagOps> flags;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (i != argc - 1) puts(("Unrecognised argument: '" + std::string(argv[i]) + "'").c_str());
            continue;
        }
        bool valid = false;
        for (uint16_t j = 0; j < fArgs; j++) {
            if (
                strcmp(fActions[j].flag.c_str(), argv[i] + 1) == 0 ||
                strcmp(fActions[j].alias.c_str(), argv[i] + 1) == 0
            ) {
                if (fActions[j].isAct) {
                    fActions[j].fAct();
                    exit(0);
                }
                else {
                    valid = true;
                    const std::string fStr = std::string(argv[i]).substr(1);
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        flags.push_back({fStr, argv[i + 1]});
                        i++; // Skip the next argument
                    }
                    else flags.push_back({fStr, ""});
                }
            }
        }
        if (!valid) puts((std::string("Invalid flag '") += argv[i] + std::string("'")).c_str());
    }
    return flags;
}
