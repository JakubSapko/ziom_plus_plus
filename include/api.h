#pragma once
#include <config.h>
#include <curl/curl.h>
#include <json.hpp>
#include <string>

using json = nlohmann::json;

class API {

public:
  API(Config *cfg);
  json get();

private:
  CURL *curl;
  Config *config;
  std::string url;
  static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output);
  void setupCurl(CURL *curl, const std::string &url,
                 std::string &responseBuffer, curl_slist &headers);
};
