use clap::Parser;
use libft_docs::parser::LibftParser;
use std::fs;

#[derive(Parser)]
#[command(name = "doc-generator")]
#[command(about = "Generate documentation for libft")]
struct Args {
    #[arg(short, long, default_value = ".")]
    source: String,
    #[arg(short, long, default_value = "dist")]
    output: String,
}

fn main() -> anyhow::Result<()> {
    let args = Args::parse();
    println!("🔍 Parsing libft source code from: {}", args.source);
    let parser = LibftParser::new(args.source);
    let metadata = parser.parse()?;
    println!("📝 Found {} functions in {} categories", metadata.functions.len(), metadata.categories.len());
    fs::create_dir_all(&args.output)?;
    let metadata_json = serde_json::to_string_pretty(&metadata)?;
    fs::write(format!("{}/metadata.json", args.output), metadata_json)?;
    println!("✅ Documentation generated in: {}", args.output);
    Ok(())
}
