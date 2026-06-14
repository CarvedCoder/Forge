use std::process::Command;

use crate::abi::error::AbiError;

pub fn extract_symbols(path: &str) -> Result<Vec<String>, AbiError> {
    let output = Command::new("nm")
        .arg("-C") 
        .arg(path)
        .output()
        .map_err(|_| AbiError::CommandError)?;

    if !output.status.success() {
        return Err(AbiError::CommandError);
    }

    let stdout = String::from_utf8_lossy(&output.stdout);

    let symbols = stdout
        .lines()
        .filter_map(|line| {
            line.split_whitespace().last()
        })
        .map(|sym| sym.to_string())
        .collect();

    Ok(symbols)
}