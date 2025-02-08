#include "utils.h"
#include <cstdlib>
#include <string>

std::string getUser() {
#ifdef _WIN32
  const char *username = std::getenv("USERNAME");
#else
  const char *username = std::getenv("USER");
#endif
  return username ? std::string(username) : "Ziombel";
}
