use crate::abi::types::AbiType;

pub fn detect_abi_type(symbols: &[String]) -> AbiType {
    if symbols.is_empty() {
        return AbiType::Unstable;
    }

    let has_cpp = symbols.iter().any(|sym| sym.as_str().starts_with("_Z"));

    if has_cpp {
        AbiType::Unstable 
    } else {
        AbiType::Stable 
    }
}