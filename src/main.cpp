#include <CLI11.hpp>

int main(int argc, char **argv) {
  CLI::App app{"Welcome to Ziom++, the faster (and probably less beautiful) "
               "version of Ziom CLI!"};
  argv = app.ensure_utf8(argv);
  std::string filename = "asd";
  app.add_option("-f,--file", filename, "A help string");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }
  return 0;
}
