#ifndef cmdutils_hpp
#define cmdutils_hpp

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <vector>

#include "Structs.h"

// Log levels
#define LOG_VERBOSE 0
#define LOG_DEBUG   1
#define LOG_INFO    2
#define LOG_WARN    3
#define LOG_ERROR   4
#define LOG_FATAL   5

// Log strings
#define VERBOSE_STR "💬 Verbose"
#define DEBUG_STR   "🐛 Debug"
#define INFO_STR    "📌 Info"
#define WARN_STR    "⚠️ Warning"
#define ERROR_STR   "❌ ERROR"
#define FATAL_STR   "☠️  FATAL"

// Log colors
#define VERBOSE_COL "\u001b[30;1m" // Grey
#define DEBUG_COL   "\u001b[34;1m" // Blue
#define INFO_COL    "\u001b[32;1m" // Green
#define WARN_COL    "\u001b[33;1m" // Yellow
#define ERROR_COL   "\u001b[35;1m" // Magenta
#define FATAL_COL   "\u001b[31;1m" // Red

// Version strings
#define VERSION_STR "0.5.0"

// Functions
void writeMsg(const std::string & msg, uint8_t level);

void writeBanner();

void writeHelp();

void writeGitHub();

std::vector<FlagOps> parseArgs(int argc, char** argv, FlagActions fActions[], uint16_t fArgs);

#endif