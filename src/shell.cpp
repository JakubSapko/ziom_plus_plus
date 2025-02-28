#include "shell.h"

#include <string>
#include <sys/wait.h> // waitpid
#include <unistd.h>   // fork, pipe, dup2, execvp
#include <vector>

std::string execCommand(const std::vector<std::string> &command) {
  int pipefd[2]; // File descriptors for the pipe
                 //
  if (pipe(pipefd) == -1) {
    throw std::runtime_error("Failed to create pipe");
  }

  pid_t pid = fork();
  if (pid == -1) {
    throw std::runtime_error("Failed to fork");
  }

  if (pid == 0) {                   // Child process
    close(pipefd[0]);               // Close read end
    dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
    dup2(pipefd[1], STDERR_FILENO); // Redirect stderr to pipe
    close(pipefd[1]);               // Close write end

    // Convert vector<string> to char* array for execvp
    std::vector<char *> args;

    for (const auto &arg : command) {
      args.push_back(const_cast<char *>(arg.c_str()));
    }
    // execvp needs this to know it's ready for command execution
    args.push_back(nullptr);

    execvp(args[0], args.data());
    _exit(1); // If execvp fails
  }

  // Parent process
  close(pipefd[1]); // Close write end

  std::string output;
  char buffer[128];
  ssize_t bytesRead;
  while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytesRead] = '\0';
    output += buffer;
  }

  close(pipefd[0]); // Close read end
  int status;
  waitpid(pid, &status, 0); // Wait for child to finish

  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    throw std::runtime_error("Command failed with exit code " +
                             std::to_string(WEXITSTATUS(status)));
  }

  return output;
}
