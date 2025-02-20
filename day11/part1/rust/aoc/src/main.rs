use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
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

    for i in 0..25 {
        blink(&mut input);
    }

    println!("Result: {}", input.len());
}

fn print_stones(input: &Vec<u64>) {
    print!("Stones: ");
    for stone in input {
        print!("{} ", stone);
    }
    println!();
}

fn blink(input: &mut Vec<u64>) {
    for i in 0..input.len() {
        if input[i] == 0 {
            input[i] = 1;
        } else if num_digits(input[i]) % 2 != 0 {
            input[i] *= 2024;
        } else {
            // split into two stones
            split(input, i);
        }
    }
}

fn num_digits(value: u64) -> u64 {
    let mut digits: u64 = 0;
    let mut val = value;

    if value == 0 {
        return 1;
    }

    while val > 0 {
        digits += 1;
        val /= 10;
    }

    return digits;
}

fn split(input: &mut Vec<u64>, index: usize) {
    let digits = num_digits(input[index]);
    let mut value = input[index];
    let mut value_hi = 0;
    let mut value_lo = 0;

    let mut mag: u64 = 1;

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

    input[index] = value_lo;
    input.push(value_hi);
}
