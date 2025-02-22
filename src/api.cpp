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

  // HEADERS
  struct curl_slist *headers = nullptr;
  std::string auth = "Authorization: Bearer";
  std::string key = this->config->apiKey;
  std::string authHeader = auth + " " + key;
  std::string applicationHeader = "Content-Type: application/json";

  headers = curl_slist_append(headers, authHeader.c_str());
  headers = curl_slist_append(headers, applicationHeader.c_str());

  // DATA
  json req = {
      {"model", "gpt-4o-mini"},
      {"messages", {{{"role", "user"}, {"content", "say this is a test"}}}},
      {"temperature", 0.7}};
  std::string reqBody = req.dump();
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, reqBody.c_str());

  this->setupCurl(this->curl, this->url, responseBuffer, *headers);

  CURLcode res = curl_easy_perform(this->curl);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK) {
    std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    return json();
  }

  try {
    return json::parse(responseBuffer);
  } catch (json::parse_error &e) {
    std::cerr << "JSON parse error: " << e.what() << "\n";
    return json();
  }
}

size_t API::writeCallback(void *contents, size_t size, size_t nmemb,
                          std::string *output) {
  size_t totalSize = size * nmemb;
  output->append((char *)contents, totalSize);
  return totalSize;
}

void API::setupCurl(CURL *curl, const std::string &url,
                    std::string &responseBuffer, curl_slist &headers) {
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, API::writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, &headers);
}
