#include "git.h"
#include "shell.h"
#include <iostream>

GitHandler::GitHandler() {
  bool isRepo{GitHandler::checkIfInRepo()};
  this->isRepo = isRepo;

  if (!(this->isRepo)) {
    throw std::runtime_error(
        "You tried initializing Ziom in a directory which is not a Git Repo!");
  }

  std::cout << "Ziom in repo";
}

bool GitHandler::checkIfInRepo() {
  try {
    std::string res =
        execCommand({"git", "rev-parse", "--is-inside-work-tree"});
    return true;
  } catch (const std::exception &e) {
    return false;
  }
}
