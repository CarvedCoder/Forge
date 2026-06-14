pub mod types;
pub mod error;
pub mod extractor;
pub mod hasher;

use extractor::extract_symbols;
use hasher::generate_abi_hash;

pub fn analyze_binary(path: &str) -> Result<types::AbiReport, error::AbiError> {
    let symbols = extract_symbols(path)?;

    let abi_hash = generate_abi_hash(&symbols);

    println!("Symbols: {:?}", symbols);
    println!("ABI Hash: {}", abi_hash);

    Err(error::AbiError::NotImplemented)
}