// #include "api.h"
#include "config_manager.h"
#include "git.h"
// #include "ziom.h"
#include <CLI11.hpp>

void handle_config(std::string key, ConfigManager cfg_manager) {

  if (key.empty()) {
    json cfg_data = cfg_manager.deserialize_config();

    if (cfg_data.is_null()) {
      return;
    }

    Config cfg = cfg_manager.create_config(cfg_data);
    std::cout << "Hello " << cfg.username
              << ", your OpenAI API key is: " << cfg.apiKey
              << ". I hope you'll have fun using Ziom!\n";
    return;
  }

  try {
    Config cfg = cfg_manager.create_config(key);
    bool gotSerialized = cfg_manager.serialize_config(cfg);
    if (!gotSerialized) {
      std::cerr << "Couldn't serialize your config, sorry!\n";
      return;
    }
  } catch (std::exception &e) {
    std::cerr << "Couldn't create your config, sorry!\n";
    return;
  }

  return;
}

void handle_default(ConfigManager &cfg_manager) {
  auto cfg = cfg_manager.get_config();
  std::cout << "hendl difolt" << *cfg << "\n";

  if (!cfg) {
    std::cerr << "Your config does not exist! Please first run 'ziom config "
                 "<API_KEY>' to set up your OpenAI API key!\n";
    exit(EXIT_FAILURE);
  }
}

void handle_change(GitHandler &git_handler) {
  try {
    git_handler.amendMessage();
    return;
  } catch (std::exception &e) {
    std::cerr << "Couldn't amend your message, sorry!\n";
    return;
  }
};

int main(int argc, char *argv[]) {
  // Dependencies init
  ConfigManager config_manager;

  // API api;
  GitHandler git_handler;
  // Ziom ziom(api, git_handler);

  // App init
  CLI::App app{"Welcome to Ziom++, the faster (and probably less beautiful) "
               "version of Ziom CLI!"};

  // Config
  auto config = app.add_subcommand(
      "config", "Get your current config. Run with additional argument (e.g. "
                "ziom config <KEY>) to setup your OpenAI API Key");
  std::string key;
  config->add_option("API_KEY", key, "OpenAI API Key");
  config->callback(
      [&key, &config_manager]() { handle_config(key, config_manager); });

  // Change
  auto change = app.add_subcommand(
      "change", "Provides the amend window for your current commit message");
  change->callback([&git_handler]() { handle_change(git_handler); });

  // ziom
  // app.callback([&config_manager]() { handle_default(config_manager); });

  CLI11_PARSE(app, argc, argv);
  return EXIT_SUCCESS;
}
