use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;
use std::collections::HashMap;

fn main() {
    let file = File::open("../../input.txt").expect("file not found");
    let reader = BufReader::new(file);
    let mut input: Vec<u64> = Vec::new();
    let mut result: u64 = 0;
    let mut stones_0: HashMap<u64, u64> = HashMap::new();
    let mut stones_1: HashMap<u64, u64> = HashMap::new();
    let mut stones_0_input = true;

    for line in reader.lines() {
        input = line
            .as_ref()
            .unwrap()
            .split(" ")
            .filter_map(|num| u64::from_str(num).ok())
            .collect();
    }

    for stone in &input {
        stones_0.insert(*stone, 1);
    }

    print_stones(&input);

    for _i in 0..75 {
        if stones_0_input {
           for (key, val) in &mut stones_0 {
               blink(*key, *val, &mut stones_1);
               *val = 0;
           }
        } else {
           for (key, val) in &mut stones_1 {
               blink(*key, *val, &mut stones_0);
               *val = 0;
           }
        }
        stones_0_input = !stones_0_input;

        //println!("{:?}", stones_0);
        //println!("{:?}", stones_1);
    }

    if stones_0_input {
        for val in stones_0.values() {
            result += val;
        }
    } else {
        for val in stones_1.values() {
            result += val;
        }
    }

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

fn blink(key: u64, val: u64, out_stones: &mut HashMap<u64, u64>) {
    let mut new_value1;
    let mut new_value2 = 0;

    if key != 0 {
        let digits = num_digits(key);

        if digits % 2 != 0 {
            new_value1 = key * 2024;
            if let Some(stone) = out_stones.get_mut(&new_value1) {
                *stone += val;
            } else {
                out_stones.insert(new_value1, val);
            }
        } else {
            // split int two stones
            new_value1 = key;
            split(&mut new_value1, &mut new_value2, digits);

            if let Some(stone) = out_stones.get_mut(&new_value1) {
                *stone += val;
            } else {
                out_stones.insert(new_value1, val);
            }

            if let Some(stone) = out_stones.get_mut(&new_value2) {
                *stone += val;
            } else {
                out_stones.insert(new_value2, val);
            }
        }
    } else {
        new_value1 = 1;
        if let Some(stone) = out_stones.get_mut(&new_value1) {
            *stone += val;
        } else {
            out_stones.insert(new_value1, val);
        }
    }
}
