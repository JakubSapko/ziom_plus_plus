#pragma once
#include <config.h>
#include <filesystem>
#include <json.hpp>

using json = nlohmann::json;

class ConfigManager {

public:
  ConfigManager();

  json deserialize_config(const std::filesystem::path file_path);
  bool serialize_config();
  json user_create_config();

private:
  Config config;
  std::filesystem::path filePath;

  bool create_config_file(const std::filesystem::path &file_path);
  std::filesystem::path create_ziom_path();
  Config create_config(const json &cfg_data);
};
