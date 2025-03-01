#include "git.h"
#include "shell.h"

GitHandler::GitHandler() {
  bool isRepo{GitHandler::checkIfInRepo()};
  this->isRepo = isRepo;

  if (!(this->isRepo)) {
    throw std::runtime_error(
        "You tried initializing Ziom in a directory which is not a Git Repo!");
  }
}

bool GitHandler::checkIfInRepo() {
  try {
    std::string _{execCommand({"git", "rev-parse", "--is-inside-work-tree"})};
    return true;
  } catch (const std::exception &e) {
    return false;
  }
}

std::string GitHandler::getGitDifference() {
  try {
    std::string res{execCommand({"git", "diff", "--staged"})};
    return res;
  } catch (const std::exception &e) {
    throw std::runtime_error(
        "Something went wrong when trying to get staged diff!");
  }
}

void GitHandler::pushChanges() {
  try {
    std::string _{execCommand({"git", "push", "origin", "master"})};
  } catch (const std::exception &e) {
    throw std::runtime_error(
        "Something went wrong when trying to push changes!");
  }
}

void GitHandler::amendMessage() {
  try {
    runInteractiveCommand({"git", "commit", "--amend"});
  } catch (const std::exception &e) {
    throw std::runtime_error("Couldn't invoke amend!");
  }
}
