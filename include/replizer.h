//
//  replizer.h
//  replizer
//
//  Created by Laurence Trippen on 26.07.18.
//  Copyright © 2018 Laurence Trippen. All rights reserved.
//

#ifndef replizer_h
#define replizer_h

#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace replizer {
class Repl {
private:
  std::string prefix;
  std::string exitCommand;
  std::function<void()> onExit = nullptr;
  std::map<std::string, std::function<void()>> callbackList;

public:
  void start();
  void setREPLPrefix(const std::string &prefix);
  void on(const std::string &command, std::function<void()> onExit);
  void question();
  void exit(const std::string &exitCommand);
  void exit(const std::string &exitCommand, std::function<void()> onExit);
};
} // namespace replizer

#endif /* replizer_h */
