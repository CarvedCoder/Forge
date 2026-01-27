use chrono::{DateTime, Utc};
use serde::{Deserialize, Serialize};

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct ForgeSchema {
    pub metadata: Metadata,
    pub project_info: ProjectInfo,
    pub abi: Abi,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub packages: Vec<Package>,
}

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct Metadata {
    #[serde(rename = "forge-version")]
    pub forge_version: String,
    pub generated: DateTime<Utc>,
}

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct ProjectInfo {
    #[serde(rename = "project-name")]
    pub project_name: String,
    #[serde(rename = "project-standard")]
    pub project_standard: String,
    #[serde(rename = "project-build-type")]
    pub project_build_type: String,
}

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct Abi {
    pub compiler: String,
    pub stdlib: String,
    #[serde(rename = "cxx-standard")]
    pub cxx_standard: String,
    #[serde(rename = "c-standard")]
    pub c_standard: String,
}

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct Package {
    pub name: String,
    pub version: String,
    pub source: Source,
}

#[derive(Debug, Deserialize, Serialize, Default)]
#[serde(deny_unknown_fields)]
pub struct Source {
    pub url: String,
    pub sha256: String,
    pub commitH
}
