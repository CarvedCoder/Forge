pub mod types;
pub mod error;

pub fn analyze_binary(_path: &str) -> Result<types::AbiReport, error::AbiError> {
    Err(error::AbiError::NotImplemented)
}