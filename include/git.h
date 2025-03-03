#pragma once
#include <string>

class GitHandler {
public:
  GitHandler();
  bool isRepo;
  std::string getGitDifference();
  void pushChanges();
  void amendMessage();
  void makeCommit(std::string commitMsg);

private:
  bool checkIfInRepo();
};
