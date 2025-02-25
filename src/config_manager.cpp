#include "config_manager.h"
#include "config.h"
#include "utils.h"
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
  if (!homeDir) {
    std::cerr << "HOME env variable is not set\n";
    exit(1);
  }
  std::filesystem::path homeDirPath = std::filesystem::path(homeDir);
  return homeDirPath / ".ziom";
}

bool ConfigManager::create_config_file(const std::filesystem::path &file_path) {
  std::ofstream outfile(file_path);
  if (!outfile.is_open()) {
    std::cerr << "Error creating file at: " << file_path << "\n";
    return false;
  }
  outfile.close();
  return true;
}

json ConfigManager::deserialize_config(const std::filesystem::path file_path) {
  /* Deserializes config found in '~/.ziom'.
   * If the file does not exist, it creates a new one and returns an empty json
   * object.
   * If the file was empty, it return an empty json object.
   */
  if (!std::filesystem::is_regular_file(file_path)) {
    bool fileCreatedSuccessfully = this->create_config_file(file_path);
    if (!fileCreatedSuccessfully) {
      std::cerr << "AAAAAAA";
      exit(1);
    }
    json user_data = this->user_create_config();
    return user_data;
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
  return config_data;
}

bool ConfigManager::serialize_config() {
  json config_data;
  config_data["apiKey"] = this->config.apiKey;
  config_data["username"] = this->config.username;

  std::ofstream output_file(this->filePath);
  if (!output_file.is_open()) {
    std::cerr << "Error opening file for writing\n";
    return false;
  }
  output_file << std::setw(4) << config_data << std::endl;
  output_file.close();
  return true;
}

Config ConfigManager::create_config(const json &cfg_data) {
  if (cfg_data.count("apiKey") && cfg_data.count("username")) {
    Config cfg{cfg_data["apiKey"], cfg_data["username"]};
    return cfg;
  }
  return Config{};
}

json ConfigManager::user_create_config() {
  std::string apiKey;
  std::cout << "Provide your OpenAI API key: \n";
  std::cin >> apiKey;
  if (apiKey.empty()) {
    std::cerr << "No API Key provided!\n";
    exit(0);
  }
  std::string username{getUser()};
  json user_data;
  user_data["apiKey"] = apiKey;
  user_data["username"] = username;
  return user_data;
}

Config *ConfigManager::get_config() { return &this->config; }
