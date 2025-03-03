#pragma once
#include "api.h"
#include "config_manager.h"
#include "git.h"

class Ziom {
public:
  Ziom(API &api, GitHandler &gh, ConfigManager &cfg_manager);
  std::string createCommitMessage();
  void mutateCommitMessage();
  void commit();

private:
  API &api;
  GitHandler &gh;
  ConfigManager &cfg_manager;
};
