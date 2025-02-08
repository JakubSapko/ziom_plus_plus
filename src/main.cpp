#include "config.h"
#include <CLI11.hpp>

int main(int argc, char **argv) {
  CLI::App app{"Welcome to Ziom++, the faster (and probably less beautiful) "
               "version of Ziom CLI!"};

  argv = app.ensure_utf8(argv);

  std::string api_key;
  CLI::Option &cfg_opt =
      *app.add_option("--config", api_key,
                      "An OpenAI API key that will be used to "
                      "generate the commit message")
           ->expected(0, 1);

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }
  if (cfg_opt.count() > 0) {
    if (api_key != "") {
      std::cout << "API Key provided: " << api_key << std::endl;
      Config cfg = Config(api_key);
      std::cout << cfg;
    } else {
      std::cout << "No API Key provided!" << std::endl;
    }
  }

  return 0;
}
