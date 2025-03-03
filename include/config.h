#pragma once
#include <string>

class Config {

public:
  std::string apiKey;
  std::string username;

  Config(std::string apiKey, std::string username);
  friend std::ostream &operator<<(std::ostream &os, const Config &cfg);
};
