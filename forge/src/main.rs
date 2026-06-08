mod cli;

use cli::{Cli, Commands};
use clap::Parser;

fn main() {
    let cli = Cli::parse();

    let result = match cli.command {
        Commands::Init(args) => cli::init::execute(args),
        Commands::Add(args) => cli::add::execute(args),
        Commands::Run => cli::run::execute(),
        Commands::Sync => cli::sync::execute(),
    };

    if let Err(e) = result {
        eprintln!("error: {e}");
        std::process::exit(1);
    }
}