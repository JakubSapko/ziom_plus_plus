#pragma once
#include <config.h>
#include <json.hpp>
#include <string>

using json = nlohmann::json;

class ConfigManager {
private:
  Config config;
  std::string filePath;

  bool create_config_file(std::string &file_path);
  std::filesystem::path create_ziom_path();
  Config create_config(json &cfg_data);

public:
  ConfigManager();
  Config *get_config();
  json deserialize_config(std::string &file_path);
  bool serialize_config();
};
