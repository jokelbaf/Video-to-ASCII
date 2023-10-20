#include <nlohmann/json.hpp>
#include <httplib.h>
#include <iostream>
#include <string>
#include <regex>

#include "cmd.h"

std::string youtubeUrlToId(const std::string& url);

std::string getVideoDownloadUrl(const std::string& videoUrl);

void downloadVideo(std::string & url);