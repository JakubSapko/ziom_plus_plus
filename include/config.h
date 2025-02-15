#pragma once
#include <string>

class Config {
private:
  std::string apiKey;
  std::string username;

public:
  Config();
  Config(std::string apiKey, std::string username);
  friend std::ostream &operator<<(std::ostream &os, const Config &cfg);
};
