#pragma once

#include <string>

class Config {
private:
  std::string filePath;
  std::string apiKey;
  std::string username;

public:
  Config(std::string apiKey);
  friend std::ostream &operator<<(std::ostream &os, const Config &cfg);
};
