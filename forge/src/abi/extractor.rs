use std::fs;
use object::{Object, ObjectSymbol};
use crate::abi::error::AbiError;

pub fn extract_symbols(path: &str) -> Result<Vec<String>, AbiError> {
    let data = fs::read(path).map_err(|_| AbiError::FileReadError)?;

    let obj = object::File::parse(&*data)
        .map_err(|_| AbiError::ParseError)?;

    let symbols = obj
        .symbols()
        .filter(|sym| sym.is_global())
        .filter_map(|sym| sym.name().ok())
        .map(|name| name.to_string())
        .collect();

    Ok(symbols)

}