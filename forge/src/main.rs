use std::fs::{read_to_string, write};

use forge::forge_schema::ForgeSchema;

fn generate_toml(path: &str) {
    let schema = ForgeSchema::default();
    let toml_schema =
        toml::to_string_pretty(&schema).expect("Failed to serialize forge schema to TOML");
    write(path, toml_schema).expect("Failed to write forge.toml");
    println!("forge.toml produced successfully");
}

fn load_toml(path: &str) {
    let read_toml = read_to_string(path).expect("Failed to read forge.toml");
    let config: ForgeSchema = toml::from_str(&read_toml).expect("Failed to parse forge.toml");
    println!("{:#?}", config);
}

fn main() {
    generate_toml("../.forge/forge.toml");
    load_toml("../.forge/forge.toml");
}
