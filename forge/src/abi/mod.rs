pub mod types;
pub mod error;
pub mod extractor;
pub mod hasher;
pub mod detector;

use extractor::extract_symbols;
use hasher::generate_abi_hash;
use detector::detect_abi_type;

pub fn analyze_binary(path: &str) -> Result<types::AbiReport, error::AbiError> {
    let symbols = extract_symbols(path)?;

    let abi_hash = generate_abi_hash(&symbols);
    let abi_type = detect_abi_type(&symbols);

    Ok(types::AbiReport {
        abi_type,
        environment: types::EnvironmentAbi {
            compiler: "unknown".to_string(),
            stdlib: "unknown".to_string(),
            cxx_standard: "unknown".to_string(),
        },
        binary: types::BinaryAbi {
            symbols,
            abi_hash,
        },
    })
}