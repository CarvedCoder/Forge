pub mod add;
pub mod init;
pub mod run;
pub mod sync;

use clap::{Parser, Subcommand};

use crate::cli::add::AddArgs;
use crate::cli::init::InitArgs;

#[derive(Debug, Parser)]
#[command(name = "forge", version, about)]
pub struct Cli {
    #[command(subcommand)]
    pub command: Commands,
}

#[derive(Debug, Subcommand)]
pub enum Commands {
    Init(InitArgs),
    Add(AddArgs),
    Run,
    Sync,
}