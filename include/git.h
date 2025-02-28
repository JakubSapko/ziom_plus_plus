#pragma once
#include <string>

class GitHandler {
public:
  GitHandler();
  bool isRepo;
  std::string getGitDifference();
  void createCommit(std::string commitMsg);
  void mutateCommitMsg(std::string commitMsg);
  void pushChanges();

private:
  bool checkIfInRepo();
  std::string commitMsg;
};
