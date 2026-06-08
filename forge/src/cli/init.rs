use std::{
    fs::{create_dir_all, write},
    path::Path,
};

use chrono::Utc;
use clap::{Args, ValueEnum};

use forge::forge_schema::{Abi, ForgeSchema, Metadata, ProjectInfo};

/// Arguments accepted by `forge init`.
#[derive(Debug, Args)]
pub struct InitArgs {
    /// Target language for the new project.
    #[arg(long, value_enum)]
    pub language: Language,
}

/// Supported project languages.
#[derive(Debug, Clone, ValueEnum)]
pub enum Language {
    C,
    Cpp,
}

/// Execute `forge init`.
pub fn execute(args: InitArgs) -> Result<(), String> {
    let forge_dir = Path::new(".forge");
    let forge_toml_path = forge_dir.join("forge.toml");

    // Refuse to re-initialise if forge.toml already exists.
    if forge_toml_path.exists() {
        return Err(format!(
            "{} already exists — project is already initialised",
            forge_toml_path.display()
        ));
    }

    // Determine language-specific values.
    let (project_standard, cxx_standard, c_standard, src_filename, hello_world) = match args.language {
        Language::Cpp => (
            "cpp".to_string(),
            "c++17".to_string(),
            String::new(),
            "src/main.cpp",
            cpp_hello_world(),
        ),
        Language::C => (
            "c".to_string(),
            String::new(),
            "c11".to_string(),
            "src/main.c",
            c_hello_world(),
        ),
    };

    // Build the ForgeSchema with sensible defaults.
    let schema = ForgeSchema {
        metadata: Metadata {
            forge_version: env!("CARGO_PKG_VERSION").to_string(),
            generated: Utc::now(),
        },
        project_info: ProjectInfo {
            project_name: current_dir_name(),
            project_standard,
            project_build_type: "executable".to_string(),
        },
        abi: Abi {
            compiler: String::new(),
            stdlib: String::new(),
            cxx_standard,
            c_standard,
        },
        packages: vec![],
    };

    // Serialise to TOML.
    let toml_content = toml::to_string_pretty(&schema)
        .map_err(|e| format!("failed to serialise forge.toml: {e}"))?;

    // Create .forge/ directory.
    create_dir_all(forge_dir)
        .map_err(|e| format!("failed to create {}: {e}", forge_dir.display()))?;

    // Write .forge/forge.toml.
    write(&forge_toml_path, &toml_content)
        .map_err(|e| format!("failed to write {}: {e}", forge_toml_path.display()))?;

    // Create src/ directory.
    let src_dir = Path::new("src");
    create_dir_all(src_dir)
        .map_err(|e| format!("failed to create src/: {e}"))?;

    // Write the hello world source file.
    let src_path = Path::new(src_filename);
    if src_path.exists() {
        eprintln!(
            "warning: {} already exists, skipping template generation",
            src_path.display()
        );
    } else {
        write(src_path, hello_world)
            .map_err(|e| format!("failed to write {}: {e}", src_path.display()))?;
    }

    // Create build/ output directory.
    create_dir_all("build")
        .map_err(|e| format!("failed to create build/: {e}"))?;

    println!("initialised project '{}'", schema.project_info.project_name);
    println!("  wrote  {}", forge_toml_path.display());
    println!("  wrote  {}", src_filename);
    println!("  created build/");

    Ok(())
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Return the name of the current working directory, falling back to "project".
fn current_dir_name() -> String {
    std::env::current_dir()
        .ok()
        .and_then(|p| p.file_name().map(|n| n.to_string_lossy().into_owned()))
        .unwrap_or_else(|| "project".to_string())
}

fn cpp_hello_world() -> &'static str {
    r#"#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
"#
}

fn c_hello_world() -> &'static str {
    r#"#include <stdio.h>

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
"#
}