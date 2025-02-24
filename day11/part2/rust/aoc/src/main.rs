use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;
use std::collections::HashMap;

fn main() {
    let file = File::open("../../test.txt").expect("file not found");
    let reader = BufReader::new(file);
    let mut input: Vec<u64> = Vec::new();
    let mut result: u64 = 0;
    let mut stones: Vec<HashMap<u64, u64>> = Vec::new();
    let mut in_idx: i32 = 0;
    let mut out_idx: i32 = 0;

    stones.push(HashMap::new());
    stones.push(HashMap::new());

    for line in reader.lines() {
        input = line
            .as_ref()
            .unwrap()
            .split(" ")
            .filter_map(|num| u64::from_str(num).ok())
            .collect();
    }

    print_stones(&input);

    println!("Result: {}", result);
}

fn print_stones(input: &Vec<u64>) {
    print!("Stones: ");
    for stone in input {
        print!("{} ", stone);
    }
    println!();
}

fn num_digits(value: u64) -> u64 {
    let mut digits = 0;
    let mut val = value;

    if val == 0 {
        return 1;
    }

    while val > 0 {
        digits += 1;
        val /= 10;
    }

    return digits;
}

fn split(num1: &mut u64, num2: &mut u64, digits: u64) {
    let mut value = *num1;
    let mut value_hi = 0;
    let mut value_lo = 0;
    let mut mag = 1;

    for i in 0..digits {
        if i == digits / 2 {
            mag = 1;
        }

        if i < digits / 2 {
            value_lo += (value % 10) * mag;
            mag *= 10;
            value /= 10;
        } else {
            value_hi += (value % 10) * mag;
            mag *= 10;
            value /= 10;
        }
    }

    *num1 = value_hi;
    *num2 = value_lo;
}
