use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::Collections::HashMap;

fn main() {
    let file = File::open("../../test.txt").expect("Failed to open file.");
    let reader = BufReader::new(file);
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut input: Vec<u8> = Vec::new();
    let mut result: u64 = 0;
    let mut trailheads: HashMap<usize, HashMap<usize, bool>> = HashMap::new();

    for line in reader.lines() {

        if width == 0 {
            width = line.as_ref().unwrap().len() as i32;
        } else
        {
            assert!(width == line.as_ref().unwrap().len() as i32);
        }

        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] - b'0');
        }

        height += 1;
    }

    println!("{} x {}", width, height);
    print_map(&input, width, height);

    for i in 0..input.len() {
        if input[i] == 0 {
            println!("Result: ({}, {}) {}", i % width as usize, i / width as usize, result);
        }
    }

    println!("Result: {}", result);
}

fn print_map(map: &Vec<u8>, width: i32, height: i32) {
    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;
            print!("{}", map[idx as usize]);
        }
        println!();
    }
    println!();
}
