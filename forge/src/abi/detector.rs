use crate::abi::types::AbiType;

pub fn detect_abi_type(symbols: &[String]) -> AbiType {
    let hash_cpp = symbols.iter().any(|sym| sym.as_str().starts_with("_Z"));

    if hash_cpp {
        AbiType::Unstable
    } else {
        AbiType::Stable
    }
}