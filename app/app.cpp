#include "commands/add.h"
#include "commands/init.h"

int main(int argc, char* argv[]) {
    CLI::App app{"Forge"};
    app.require_subcommand(1); // must pick exactly one subcommand, or CLI11 errors cleanly

    forge::commands::register_init(app);
    forge::commands::register_add(app);

    CLI11_PARSE(app, argc, argv);
    return 0;
}
