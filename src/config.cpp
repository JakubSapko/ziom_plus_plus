#include "config.h"
#include <iostream>
#include <string>

Config::Config(std::string apiKey, std::string username) {
  this->apiKey = apiKey;
  this->username = username;
};

Config::Config() {
  this->apiKey = "";
  this->username = "";
}

std::ostream &operator<<(std::ostream &os, const Config &cfg) {
  return os << "{\napiKey: " << cfg.apiKey << ",\nusername: " << cfg.username
            << ",\n}";
}
