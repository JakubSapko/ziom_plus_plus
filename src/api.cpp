#include "api.h"
#include <config.h>
#include <curl/curl.h>
#include <iostream>

API::API(Config *cfg) {
  this->curl = curl_easy_init();
  if (!this->curl) {
    std::cerr << "Failed to initialize cURL\n";
    exit(1);
  }
  this->url = "https://api.openai.com/v1/chat/completions";
  this->config = cfg;
}

json API::get() {
  std::string responseBuffer;

  this->setupCurl(this->curl, this->url, responseBuffer);
  CURLcode res = curl_easy_perform(this->curl);

  if (res != CURLE_OK) {
    std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    curl_easy_cleanup(curl);
    return json();
  }

  return json::object();
}

size_t API::writeCallback(void *contents, size_t size, size_t nmemb,
                          std::string *output) {
  size_t totalSize = size * nmemb;
  output->append((char *)contents, totalSize);
  return totalSize;
}

void API::setupCurl(CURL *curl, const std::string &url,
                    std::string &responseBuffer) {
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, API::writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
}
