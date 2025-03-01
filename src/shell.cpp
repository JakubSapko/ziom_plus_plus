#include "shell.h"

#include <fcntl.h> // posix_openpt, O_RDWR
#include <string>
#include <sys/ioctl.h> // ioctl
#include <sys/wait.h>  // waitpid
#include <termios.h>
#include <unistd.h> // fork, pipe, dup2, execvp
#include <util.h>
#include <utmp.h>
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

// Enable raw mode on the terminal
void enableRawMode(int fd, struct termios &orig_termios) {
  struct termios raw;
  tcgetattr(fd, &orig_termios); // Get current terminal attributes
  raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON); // Disable echo & canonical mode
  tcsetattr(fd, TCSAFLUSH, &raw);  // Apply changes
}

// Restore original terminal settings
void disableRawMode(int fd, struct termios &orig_termios) {
  tcsetattr(fd, TCSAFLUSH, &orig_termios);
}

void runInteractiveCommand(const std::vector<std::string> &command) {
  int master_fd;
  pid_t pid = forkpty(&master_fd, nullptr, nullptr, nullptr);

  if (pid == -1) {
    throw std::runtime_error("Fork failed");
  }

  if (pid == 0) { // Child process
    // Convert command vector to execvp format
    std::vector<char *> args;
    for (const auto &arg : command) {
      args.push_back(const_cast<char *>(arg.c_str()));
    }
    args.push_back(nullptr);

    execvp(args[0], args.data());
    _exit(1);
  }

  // Parent process: Forward input/output
  fd_set fds;
  char buffer[256];
  ssize_t bytesRead;

  while (true) {
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    FD_SET(master_fd, &fds);

    int max_fd = std::max(STDIN_FILENO, master_fd);
    if (select(max_fd + 1, &fds, nullptr, nullptr, nullptr) == -1) {
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &fds)) {
      bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
      if (bytesRead > 0) {
        write(master_fd, buffer, bytesRead);
      }
    }

    if (FD_ISSET(master_fd, &fds)) {
      bytesRead = read(master_fd, buffer, sizeof(buffer));
      if (bytesRead > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
      } else {
        break; // Child process exited
      }
    }
  }

  close(master_fd);
  waitpid(pid, nullptr, 0);
}
