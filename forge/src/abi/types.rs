use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct AbiReport {
    pub abi_type: AbiType,
    pub environment: EnvironmentAbi,
    pub binary: BinaryAbi,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub enum AbiType {
    Stable,
    Unstable,
    Unknown,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct EnvironmentAbi {
    pub compiler: String,
    pub stdlib: String,
    pub cxx_standard: String,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct BinaryAbi {
    pub symbols: Vec<String>,
    pub abi_hash: String,
}