pub mod types;
pub mod error;
pub mod extractor;
pub mod hasher;
pub mod detector;

use extractor::extract_symbols;
use hasher::generate_abi_hash;
use detector::detect_abi_type;

use types::{AbiReport, EnvironmentAbi, BinaryAbi};

pub fn analyze_binary(path: &str) -> Result<AbiReport, error::AbiError> {
    let symbols = extract_symbols(path)?;

    let abi_hash = generate_abi_hash(&symbols);
    let abi_type = detect_abi_type(&symbols);

    Ok(AbiReport {
        abi_type,
        environment: EnvironmentAbi {
            compiler: None,
            stdlib: None,
            cxx_standard: None,
        },
        binary: BinaryAbi {
            symbols,
            abi_hash,
        },
    })
}