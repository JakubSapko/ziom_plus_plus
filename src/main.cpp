#include "api.h"
#include "config_manager.h"
#include "git.h"
#include "ziom.h"
#include <CLI11.hpp>

// TODO: delete replizer?
int main() {

  GitHandler gitHandler;
  ConfigManager config_manager;
  API api(config_manager.get_config());
  Ziom ziom(api, gitHandler);

  gitHandler.amendMessage();
  exit(1);
  /*CLI::App app{"Welcome to Ziom++, the faster (and probably less beautiful) "
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

  // Logic part - should it be there?
  // cfg_opt.count > 0 -> ziom --flag <something>
  if (cfg_opt.count() > 0) {
    if (api_key != "") {
      std::cout << "API Key provided: " << api_key << std::endl;
      Config cfg = Config(api_key);
      std::cout << cfg;
    } else {
      std::cout << "No API Key provided!" << std::endl;
    }
  }*/
  // count == 0 -> ziom

  return 0;
}
