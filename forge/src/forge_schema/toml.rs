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
    #[serde(rename = "project-name")]
    pub name: String,
    #[serde(rename = "project-standard")]
    pub standard: String,
    #[serde(rename = "project-build-type")]
    pub build_type: String,
}

#[derive(Debug, Serialize, Deserialize, Default)]
pub struct Package {
    pub name: String,
    pub version: String,
    pub source: String,
}
