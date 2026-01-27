use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize, Default)]
#[serde(deny_unknown_fields)]
pub struct ForgeToml {
    pub project: ProjectInfo,

    #[serde(default)]
    pub packages: Vec<Package>,
}

#[derive(Debug, Serialize, Deserialize, Default)]
pub struct ProjectInfo {
    pub name: String,
    pub standard: String,
    pub build_type: String,
}

#[derive(Debug, Serialize, Deserialize, Default)]
pub struct Package {
    pub name: String,
    pub version: String,
    pub source: String,
}
