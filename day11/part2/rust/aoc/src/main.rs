use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

fn main() {
    let file = File::open("../../test.txt").expect("file not found");
    let reader = BufReader::new(file);
    let mut input: Vec<u64> = Vec::new();

    for line in reader.lines() {
        input = line
            .as_ref()
            .unwrap()
            .split(" ")
            .filter_map(|num| u64::from_str(num).ok())
            .collect();
    }

    print_stones(&input);
}

fn print_stones(input: &Vec<u64>) {
    print!("Stones: ");
    for stone in input {
        print!("{} ", stone);
    }
    println!();
}
