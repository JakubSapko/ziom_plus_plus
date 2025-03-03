#include "ziom.h"
#include <iostream>

Ziom::Ziom(API &api, GitHandler &gh) : api(api), gh(gh) {};

std::string Ziom::createCommitMessage() {
  if (!this->gh.isRepo) {
    throw std::runtime_error(
        "Critical error! GH Handler initialized but we're not in repo!");
  }
  std::string diff = this->gh.getGitDifference();
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
