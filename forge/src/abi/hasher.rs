use sha2::{Digest, Sha256};

pub fn generate_abi_hash(symbols: &[String]) -> String {
    let mut hasher = Sha256::new();

    let mut sorted: Vec<&str> = symbols.iter().map(|s| s.as_str()).collect();
    sorted.sort();

    for sym in &sorted {
        hasher.update(sym.as_bytes());
    }

    format!("{:x}", hasher.finalize())
}