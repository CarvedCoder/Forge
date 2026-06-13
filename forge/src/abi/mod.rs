pub mod types;
pub mod error;
pub mod extractor;

use extractor::extract_symbols;

pub fn analyze_binary(path: &str) -> Result<types::AbiReport, error::AbiError> {
    let symbols = extract_symbols(path)?;

    println!("Extracted symbols: {:?}", symbols);

    Err(error::AbiError::NotImplemented)
}