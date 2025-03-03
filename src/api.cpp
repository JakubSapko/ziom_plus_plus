#include "api.h"
#include "constants.h"
#include <config.h>
#include <curl/curl.h>
#include <iostream>

API::API() {
  this->curl = curl_easy_init();
  if (!this->curl) {
    std::cerr << "Failed to initialize cURL\n";
    exit(1);
  }
  this->url = "https://api.openai.com/v1/chat/completions";
}

void API::set_config(Config &cfg) { this->config = cfg; }

json API::post(std::string_view diff) {
  std::string responseBuffer;

  curl_slist *headers = this->setupHeaders();

  // DATA
  json req = {{"model", "gpt-4o-mini"},
              {"messages",
               {{{"role", "system"}, {"content", apiConstants::system_prompt}},
                {{"role", "user"}, {"content", diff}}}},
              {"temperature", 0.7}};

  this->setupCurl(this->curl, this->url, req, responseBuffer, headers);

  CURLcode res = curl_easy_perform(this->curl);

  if (res != CURLE_OK) {
    std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
    return json();
  }

  curl_slist_free_all(headers);
  // curl_easy_cleanup(this->curl);

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
  static_cast<std::string *>(output)->append(static_cast<char *>(contents),
                                             totalSize);

  return totalSize;
}

void API::setupCurl(CURL *curl, const std::string &url, json reqBody,
                    std::string &responseBuffer, curl_slist *headers) {
  // Lifetime issue for this value
  std::string requestBody = reqBody.dump();

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  // use of COPYPOSTFIELDS solves the lifetime issue, but it's quite nasty tbh
  curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, requestBody.c_str());
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, API::writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

curl_slist *API::setupHeaders() {
  struct curl_slist *headers = nullptr;
  std::string auth = "Authorization: Bearer";
  std::string key = this->config.apiKey;
  std::string authHeader = auth + " " + key;
  std::string applicationHeader = "Content-Type: application/json";

  headers = curl_slist_append(headers, authHeader.c_str());
  headers = curl_slist_append(headers, applicationHeader.c_str());
  return headers;
}

API::~API() {
  if (this->curl) {
    curl_easy_cleanup(this->curl);
  }
}
