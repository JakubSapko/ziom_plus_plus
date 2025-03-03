#include "ziom.h"
#include <iostream>

Ziom::Ziom(API &api, GitHandler &gh, ConfigManager &cfg_manager)
    : api(api), gh(gh), cfg_manager(cfg_manager) {};

std::string Ziom::createCommitMessage() {
  if (!this->gh.isRepo) {
    throw std::runtime_error(
        "Critical error! GH Handler initialized but we're not in repo!");
  }

  std::string diff = this->gh.getGitDifference();
  json config_data = this->cfg_manager.deserialize_config();
  Config cfg = this->cfg_manager.create_config(config_data);

  this->api.set_config(cfg);
  json res = this->api.post(diff);

  if (res["choices"].size()) {
    size_t resLen{res["choices"].size()};
    std::string commitMsg{res["choices"][resLen - 1]["message"]["content"]};
    return commitMsg;
  }

  return "";
}

void Ziom::mutateCommitMessage() { this->gh.amendMessage(); };

void Ziom::commit() {
  std::string commitMsg = this->createCommitMessage();
  if (commitMsg.empty()) {
    std::cerr
        << "No commit message was created! This most likely means something's "
           "off with the OpenAI API communication! Aborting...\n";
    return;
  }
  this->gh.makeCommit(commitMsg);
}
