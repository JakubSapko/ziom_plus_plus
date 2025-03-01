#include "ziom.h"

Ziom::Ziom(API &api, GitHandler &gh) : api(api), gh(gh) {};

void Ziom::createCommitMessage() {
  if (!this->gh.isRepo) {
    throw std::runtime_error(
        "Critical error! GH Handler initialized but we're not in repo!");
  }
  std::string diff = this->gh.getGitDifference();
  json res = this->api.post(diff);

  size_t resLen{res["choices"].size()};
  std::string commitMsg{res["choices"][resLen - 1]["message"]["content"]};
  this->commitMsg = commitMsg;
}

void Ziom::mutateCommitMessage() { this->gh.amendMessage(); };
