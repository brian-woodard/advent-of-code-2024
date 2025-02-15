use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    let file = File::open("../../test.txt").expect("failed to open file");
    let reader = BufReader::new(file);

    for line in reader.lines() {
        println!("{}", line.unwrap());
    }
}
