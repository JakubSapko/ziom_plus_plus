#pragma once
#include "api.h"
#include "git.h"

class Ziom {
public:
  Ziom(API &api, GitHandler &gh);
  void createCommitMessage();
  void mutateCommitMessage();

private:
  API &api;
  GitHandler &gh;
  std::string commitMsg;
};
