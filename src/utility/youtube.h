#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <nlohmann/json.hpp>
#include <httplib.h>
#include <iostream>
#include <string>
#include <regex>

#include "cmd.h"

void downloadYouTubeVideo(const std::string & videoUrl, int maxRetries = 3);

#endif