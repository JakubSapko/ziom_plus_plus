#pragma once
#include <config.h>
#include <filesystem>
#include <json.hpp>
#include <optional>

using json = nlohmann::json;

class ConfigManager {

public:
  ConfigManager();

  json deserialize_config();
  bool serialize_config(Config &cfg);
  json compose_user_data(std::string apiKey);
  void set_config(Config cfg);
  std::optional<Config> &get_config();

  Config create_config(std::string apiKey);
  Config create_config(json cfg_data);

private:
  std::optional<Config> config;
  std::filesystem::path filePath;

  bool create_config_file(const std::filesystem::path &file_path);
  std::filesystem::path create_ziom_path();
};
