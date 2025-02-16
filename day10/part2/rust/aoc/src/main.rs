use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    let file = File::open("../../test.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let mut input: Vec<i32> = Vec::new();
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut result: u64 = 0;

    for line in reader.lines() {
        if width == 0 {
            width = line.as_ref().unwrap().len() as i32;
        } else {
            assert!(width == line.as_ref().unwrap().len() as i32);
        }

        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] as i32 - b'0' as i32);
        }

        height += 1;
    }

    println!("{} x {}", width, height);
    print_map(&input, width, height);

    for i in 0..input.len() {
        if input[i] == 0 {
            println!("check_trailhead");
        }
    }

    println!("Result: {}", result);
}

fn print_map(map: &Vec<i32>, width: i32, height: i32) {
    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;
            print!("{}", map[idx as usize]);
        }
        println!();
    }
    println!();
}
