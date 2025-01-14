use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    let file = File::open("../../test.txt").expect("failed to open file.");
    let reader = BufReader::new(file);
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut input: Vec<char> = Vec::new();

    for line in reader.lines() {
        if width == 0 {
            width = line.as_ref().unwrap().len().try_into().unwrap();
        } else {
            assert!(width == line.as_ref().unwrap().len().try_into().unwrap());
        }

        height += 1;

        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] as char);
        }
    }

    println!("{} x {}", width, height);

    println!("{:?}", input);
}
