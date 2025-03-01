#pragma once
#include <string>

class GitHandler {
public:
  GitHandler();
  bool isRepo;
  std::string getGitDifference();
  void pushChanges();
  void amendMessage();

private:
  bool checkIfInRepo();
};
