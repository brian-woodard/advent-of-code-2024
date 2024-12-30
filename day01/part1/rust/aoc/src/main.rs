
use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

fn main() {
    let file = File::open("../../input.txt").expect("file wasn't found.");
    let reader = BufReader::new(file);
    let mut list1: Vec<i32> = Vec::new();
    let mut list2: Vec<i32> = Vec::new();
    let mut total_distance: u64 = 0;

    for line in reader.lines() {
        let nums: Vec<i32> = line
            .unwrap()
            .split(" ")
            .filter_map(|num| i32::from_str(num).ok())
            .collect();
        list1.push(nums[0]);
        list2.push(nums[1]);
    }

    assert!(list1.len() > 0);
    assert!(list2.len() > 0);
    assert!(list1.len() == list2.len());

    list1.sort();
    list2.sort();

    for i in 0..list1.len() {
        total_distance += (list1[i] - list2[i]).abs() as u64;
    }

    println!("total distance: {}", total_distance);
}
