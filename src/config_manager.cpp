#include "config_manager.h"
#include "config.h"
#include "utils.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

ConfigManager::ConfigManager() {
  this->filePath = this->create_ziom_path();
  /*json config_data = this->deserialize_config(this->filePath);
  if (!(config_data.empty())) {
    this->config = this->create_config(config_data);
  }*/
}

void ConfigManager::set_config(Config cfg) { this->config.emplace(cfg); }
std::optional<Config> &ConfigManager::get_config() { return this->config; }

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

json ConfigManager::deserialize_config() {
  /* Deserializes config found in '~/.ziom'. */
  const std::filesystem::path file_path = this->create_ziom_path();

  std::ifstream input_file(file_path);
  if (!input_file.is_open()) {
    std::cerr << "Error opening file for reading. Most likely your config does "
                 "not exist! Please run 'ziom config' first!"
              << "\n";
    return json::object();
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

bool ConfigManager::serialize_config(Config &cfg) {
  json config_data;
  config_data["apiKey"] = cfg.apiKey;
  config_data["username"] = cfg.username;

  std::ofstream output_file(this->filePath);
  if (!output_file.is_open()) {
    std::cerr << "Error opening file for writing\n";
    return false;
  }
  output_file << std::setw(4) << config_data << std::endl;
  output_file.close();
  return true;
}

Config ConfigManager::create_config(std::string apiKey) {
  if (apiKey.empty()) {
    throw std::runtime_error(
        "Couldn't create config due to the missing API key!");
  }
  std::string username{getUser()};
  if (username.empty()) {
    throw std::runtime_error(
        "Couldn't create config due to the missing username!");
  }
  Config cfg{apiKey, username};
  return cfg;
}

Config ConfigManager::create_config(json cfg_data) {
  if (!(cfg_data.count("apiKey"))) {
    throw std::runtime_error(
        "Couldn't create config due to the missing API key!");
  }
  if (!(cfg_data.count("username"))) {
    throw std::runtime_error(
        "Couldn't create config due to the missing username!");
  }
  Config cfg{cfg_data["apiKey"], cfg_data["username"]};
  return cfg;
}
