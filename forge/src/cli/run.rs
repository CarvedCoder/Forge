use std::{
    fs::read_to_string,
    path::Path,
    process::Command,
};

use forge::forge_schema::ForgeSchema;

const FORGE_TOML: &str = ".forge/forge.toml";

pub fn execute() -> Result<(), String> {
    let path = Path::new(FORGE_TOML);

    if !path.exists() {
        return Err(format!(
            "{FORGE_TOML} not found — run `forge init` first"
        ));
    }

    let raw = read_to_string(path)
        .map_err(|e| format!("failed to read {FORGE_TOML}: {e}"))?;

    let schema: ForgeSchema = toml::from_str(&raw)
        .map_err(|e| format!("failed to parse {FORGE_TOML}: {e}"))?;

    let standard = schema.project_info.project_standard.to_lowercase();
    let language = detect_language(&standard)?;

    let src_file = match language {
        Language::Cpp => "src/main.cpp",
        Language::C => "src/main.c",
    };

    if !Path::new(src_file).exists() {
        return Err(format!(
            "entry point '{src_file}' not found — has the project been initialised correctly?"
        ));
    }

    let compiler = find_compiler(language)?;

    std::fs::create_dir_all("build")
        .map_err(|e| format!("failed to create build/: {e}"))?;

    let project_name = &schema.project_info.project_name;

    // Platform-specific binary name: .exe on Windows, bare name on Unix.
    let binary_name = if cfg!(target_os = "windows") {
        format!("{project_name}.exe")
    } else {
        project_name.clone()
    };
    let binary_path = format!("build/{binary_name}");

    let std_flag = match language {
        Language::Cpp => abi_std_flag(&schema.abi.cxx_standard, "c++17"),
        Language::C => abi_std_flag(&schema.abi.c_standard, "c11"),
    };

    let compile_args: Vec<String> = vec![
        std_flag,
        src_file.to_string(),
        "-o".to_string(),
        binary_path.clone(),
    ];

    println!("compiling with {compiler}...");
    println!("  {} {}", compiler, compile_args.join(" "));

    let compile_status = Command::new(&compiler)
        .args(&compile_args)
        .status()
        .map_err(|e| format!("failed to launch compiler '{compiler}': {e}"))?;

    if !compile_status.success() {
        let code = compile_status
            .code()
            .map(|c| c.to_string())
            .unwrap_or_else(|| "unknown".to_string());
        return Err(format!("compilation failed (exit code: {code})"));
    }

    println!("compiled successfully → {binary_path}");
    println!("running {binary_path}...\n");

    // On Unix the path must be qualified so the OS does not search PATH.
    // On Windows, Command::new resolves relative paths natively without ./
    let run_command = if cfg!(target_os = "windows") {
        binary_path.clone()
    } else {
        format!("./{binary_path}")
    };

    let run_status = Command::new(&run_command)
        .status()
        .map_err(|e| format!("failed to launch '{run_command}': {e}"))?;

    if !run_status.success() {
        let code = run_status
            .code()
            .map(|c| c.to_string())
            .unwrap_or_else(|| "unknown".to_string());
        return Err(format!(
            "program exited with non-zero status (exit code: {code})"
        ));
    }

    Ok(())
}

// ---------------------------------------------------------------------------
// Internal types
// ---------------------------------------------------------------------------

#[derive(Clone, Copy)]
enum Language {
    C,
    Cpp,
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

fn detect_language(standard: &str) -> Result<Language, String> {
    match standard {
        "cpp" | "c++" => Ok(Language::Cpp),
        "c" => Ok(Language::C),
        other => Err(format!(
            "unrecognised project-standard '{other}' in {FORGE_TOML} — expected 'cpp' or 'c'"
        )),
    }
}

fn find_compiler(language: Language) -> Result<String, String> {
    let candidates: &[&str] = match language {
        Language::Cpp => &["clang++", "g++"],
        Language::C => &["clang", "gcc"],
    };

    for &candidate in candidates {
        if compiler_available(candidate) {
            return Ok(candidate.to_string());
        }
    }

    Err(format!(
        "no suitable compiler found — tried: {}",
        candidates.join(", ")
    ))
}

fn compiler_available(compiler: &str) -> bool {
    Command::new(compiler)
        .arg("--version")
        .stdout(std::process::Stdio::null())
        .stderr(std::process::Stdio::null())
        .status()
        .map(|s| s.success())
        .unwrap_or(false)
}

fn abi_std_flag(field: &str, default: &str) -> String {
    let value = if field.trim().is_empty() { default } else { field };
    format!("-std={value}")
}