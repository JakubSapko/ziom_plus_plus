#include "config_manager.h"
#include "config.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

ConfigManager::ConfigManager() {
  this->filePath = this->create_ziom_path();
  json config_data = this->deserialize_config(this->filePath);
  this->config = this->create_config(config_data);
}

std::filesystem::path ConfigManager::create_ziom_path() {
  const char *homeDir = std::getenv("HOME");
  std::filesystem::path homeDirPath = std::filesystem::path(homeDir);
  return homeDirPath / ".ziom";
}

bool ConfigManager::create_config_file(std::string &file_path) {
  std::ofstream outfile(file_path);
  if (!outfile.is_open()) {
    std::cerr << "Error creating file at: " << file_path << "\n";
    return 0;
  }
  outfile.close();
  return 1;
}

json ConfigManager::deserialize_config(std::string &file_path) {
  /* Deserializes config found in '~/.ziom'.
   * If the file does not exist, it creates a new one and returns an empty json
   * object.
   * If the file was empty, it return an empty json object.
   */
  if (!std::filesystem::is_regular_file(file_path)) {
    this->create_config_file(file_path);
    return json::object();
  }

  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    std::cerr << "Error opening file for reading" << "\n";
    exit(0);
  }

  json config_data;
  try {
    input_file >> config_data;
  } catch (json::parse_error &e) {
    if (e.id == 101) {
      config_data = json::object();
    }
  } catch (json::type_error &e) {
    std::cerr << "JSON type error: " << e.what() << "\n";
  }
  input_file.close();
  std::cout << config_data;
  return config_data;
}

Config ConfigManager::create_config(json &cfg_data) {
  Config cfg{};
  if (cfg_data.count("apiKey") && cfg_data.count("username")) {
    Config cfg{cfg_data["apiKey"], cfg_data["username"]};
    return cfg;
  }
  return cfg;
}

Config *ConfigManager::get_config() { return &this->config; }
