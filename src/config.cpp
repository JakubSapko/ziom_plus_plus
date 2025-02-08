#include "config.h"
#include "utils.h"
#include <iostream>
#include <string>

Config::Config(std::string key) {
  this->apiKey = key;
  this->username = getUser();
  this->filePath = "~";
};

std::ostream &operator<<(std::ostream &os, const Config &cfg) {
  return os << "{\napiKey: " << cfg.apiKey << ",\nusername: " << cfg.username
            << ",\nfilePath: " << cfg.filePath << ",\n}";
}
