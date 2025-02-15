#include "config.h"
#include <iostream>
#include <string>

Config::Config(std::string apiKey, std::string username)
    : apiKey{std::move(apiKey)}, username{std::move(username)} {}

Config::Config() : apiKey{""}, username{""} {}

std::ostream &operator<<(std::ostream &os, const Config &cfg) {
  return os << "{\napiKey: " << cfg.apiKey << ",\nusername: " << cfg.username
            << ",\n}";
}
