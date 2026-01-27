use chrono::{DateTime, Utc};
use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize)]
pub struct ForgeLock {
    pub metadata: Metadata,
    pub abi: Abi,
    pub resolved_packages: Vec<ResolvedPackage>,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct Metadata {
    pub forge_version: String,
    pub generated: DateTime<Utc>,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct Abi {
    pub compiler: String,
    pub stdlib: String,
    pub cxx_standard: String,
    pub c_standard: String,
}

#[derive(Debug, Serialize, Deserialize)]
pub struct ResolvedPackage {
    pub name: String,
    pub version: String,
    pub source_url: String,
    pub sha256: String,
}
