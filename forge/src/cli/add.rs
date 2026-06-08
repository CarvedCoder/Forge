use std::{
    fs::{read_to_string, write},
    path::Path,
};

use clap::Args;

use forge::forge_schema::{ForgeSchema, Package, GitSource};

/// Arguments accepted by `forge add`.
#[derive(Debug, Args)]
pub struct AddArgs {
    /// Name of the package to add.
    pub name: String,
}

const FORGE_TOML: &str = ".forge/forge.toml";

/// Execute `forge add <package>`.
pub fn execute(args: AddArgs) -> Result<(), String> {
    let path = Path::new(FORGE_TOML);

    // Ensure the project has been initialised.
    if !path.exists() {
        return Err(format!(
            "{} not found — run `forge init` first",
            FORGE_TOML
        ));
    }
    
    if args.name.trim().is_empty() {
    return Err("package name cannot be empty".to_string());}


    // Read and parse the existing forge.toml.
    let raw = read_to_string(path)
        .map_err(|e| format!("failed to read {FORGE_TOML}: {e}"))?;

    let mut schema: ForgeSchema = toml::from_str(&raw)
        .map_err(|e| format!("failed to parse {FORGE_TOML}: {e}"))?;

    // Reject duplicate package names.
    if schema.packages.iter().any(|p| p.name == args.name) {
        return Err(format!(
            "package '{}' is already listed in {FORGE_TOML}",
            args.name
        ));
    }

    // Append the new package with placeholder fields.
    // URL, sha256, and commit_hash are left empty; `forge sync` will resolve them.
    schema.packages.push(Package {
        name: args.name.clone(),
        version: String::new(),
        source: GitSource {
            url: String::new(),
            sha256: String::new(),
            commit_hash: String::new(),
        },
    });

    // Serialise the updated schema back to TOML.
    let updated = toml::to_string_pretty(&schema)
        .map_err(|e| format!("failed to serialise {FORGE_TOML}: {e}"))?;

    write(path, updated)
        .map_err(|e| format!("failed to write {FORGE_TOML}: {e}"))?;

    println!("added package '{}'", args.name);
    println!("  note: run `forge sync` to resolve and fetch dependencies");

    Ok(())
}