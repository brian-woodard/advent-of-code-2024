use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;
use std::collections::HashMap;

fn main() {
    let file = File::open("../../input.txt").expect("file not found.");
    let reader = BufReader::new(file);
    let mut list1: Vec<i32> = Vec::new();
    let mut list2: Vec<i32> = Vec::new();
    let mut similarity_map: HashMap<i32, u64> = HashMap::new();
    let mut similarity : u64 = 0;

    for line in reader.lines() {
        let nums : Vec<i32> = line
            .unwrap()
            .split(" ")
            .filter_map(|num| i32::from_str(num).ok())
            .collect();

        list1.push(nums[0]);
        list2.push(nums[1]);

        if let Some(map_num) = similarity_map.get_mut(&nums[1]) {
            *map_num += 1;
        } else {
            similarity_map.insert(nums[1], 1);
        }
    }

    assert!(list1.len() == list2.len());

    for num in list1 {
        if let Some(sim_value) = similarity_map.get(&num) {
            similarity += sim_value * num as u64;
        }
    }

    println!("similarity: {}", similarity);
}
