#pragma once
#include "api.h"
#include "git.h"

class Ziom {
public:
  Ziom(API &api, GitHandler &gh);
  std::string createCommitMessage();
  void mutateCommitMessage();
  void commit();

private:
  API &api;
  GitHandler &gh;
};
