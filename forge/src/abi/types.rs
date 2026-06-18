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
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct EnvironmentAbi {
    #[serde(skip_serializing_if = "Option::is_none")]
    pub compiler: Option<String>,

    #[serde(skip_serializing_if = "Option::is_none")]
    pub stdlib: Option<String>,

    #[serde(skip_serializing_if = "Option::is_none")]
    pub cxx_standard: Option<String>,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct BinaryAbi {
    pub symbols: Vec<String>,
    pub abi_hash: String,
}