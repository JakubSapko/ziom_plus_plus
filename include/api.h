#pragma once
#include <config.h>
#include <curl/curl.h>
#include <json.hpp>
#include <string>

using json = nlohmann::json;

class API {

public:
  API();
  ~API();
  json post(std::string_view diff);
  void set_config(Config &cfg);

private:
  CURL *curl;
  std::optional<Config> config;
  std::string url;
  static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output);
  void setupCurl(CURL *curl, const std::string &url, json reqBody,
                 std::string &responseBuffer, curl_slist *headers);
  curl_slist *setupHeaders();
};
