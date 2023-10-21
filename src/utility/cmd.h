#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <cstdio>
#include <string>
#include <chrono>
#include <vector>

#include "src/structs.h"

// Log levels
#define LOG_DEBUG   0
#define LOG_INFO    1
#define LOG_WARN    2
#define LOG_ERROR   3
#define LOG_FATAL   4

// Log strings
#define DEBUG_STR   "DEBUG"
#define INFO_STR    "INFO"
#define WARN_STR    "WARNING"
#define ERROR_STR   "ERROR"
#define FATAL_STR   "FATAL"

// Log colors
#define GREY_COL    "\u001b[30;1m" // Grey
#define DEBUG_COL   "\u001b[34;1m" // Blue
#define INFO_COL    "\u001b[32;1m" // Green
#define WARN_COL    "\u001b[33;1m" // Yellow
#define ERROR_COL   "\u001b[35;1m" // Magenta
#define FATAL_COL   "\u001b[31;1m" // Red

// Version strings
#define VERSION_STR "1.0.0"

// Functions
void writeMsg(const std::string & msg, uint8_t level);

void writeBanner();

void writeHelp();

void writeGitHub();

std::vector<FlagOps> parseArgs(int argc, char** argv, FlagActions fActions[], uint16_t fArgs);

#endif