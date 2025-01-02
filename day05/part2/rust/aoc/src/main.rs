use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;
use multimap::MultiMap;

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file.");
    let reader = BufReader::new(file);
    let mut store_rules = true;
    let mut input: Vec<Vec<i32>> = Vec::new();
    let mut before_map: MultiMap<i32, i32> = MultiMap::new();
    let mut after_map: MultiMap<i32, i32> = MultiMap::new();
    let mut result = 0;

    for line in reader.lines() {
        if store_rules {
            if line.as_ref().unwrap().len() == 0 {
                store_rules = false;
            } else {
                let nums: Vec<i32> = line
                    .as_ref()
                    .unwrap()
                    .split("|")
                    .filter_map(|num| i32::from_str(num).ok())
                    .collect();

                if nums.len() == 2 {
                    before_map.insert(nums[0], nums[1]);
                    after_map.insert(nums[1], nums[0]);
                }
            }
        } else {
            let nums: Vec<i32> = line
                .as_ref()
                .unwrap()
                .split(",")
                .filter_map(|num| i32::from_str(num).ok())
                .collect();
            input.push(nums);
        }
    }

    for mut pages in input {
        let mut mid = check_order(&mut pages, &before_map, &after_map);
        if mid == 0 {
            fix_order(&mut pages, &before_map);
            mid = check_order(&mut pages, &before_map, &after_map);
            result += mid;
        }
    }

    println!("Result: {}", result);
}

fn check_order(pages: &mut Vec<i32>, before_map: &MultiMap<i32, i32>, after_map: &MultiMap<i32, i32>) -> i32 {
    let mut result = 0;
    let mut found = true;

    for i in 0..pages.len() {
        for j in 0..i {
            if !check_in_range(pages[i], pages[j], after_map) {
                found = false;
                break;
            }
        }

        for j in i+1..pages.len() {
            if !check_in_range(pages[i], pages[j], before_map) {
                found = false;
                break;
            }
        }

        if !found {
            break;
        }
    }

    if found {
        let mid = pages.len() / 2;
        result += pages[mid];
    }

    return result;
}

fn fix_order(pages: &mut Vec<i32>, before_map: &MultiMap<i32, i32>) {
    let mut new_pages: Vec<i32> = Vec::new();

    new_pages.reserve(pages.len());

    new_pages.push(pages[0]);
    for i in 1..pages.len() {
        let mut placed = false;

        for j in 0..new_pages.len() {
            if check_in_range(pages[i], new_pages[j], before_map) {
                new_pages.insert(j, pages[i]);
                placed = true;
                break;
            }
        }

        if !placed {
            new_pages.push(pages[i]);
        }
    }

    *pages = new_pages;
}

fn check_in_range(key: i32, value: i32, map: &MultiMap<i32, i32>) -> bool {
    let mut result = false;

    if let Some(values) = map.get_vec(&key) {
        for val in values {
            if *val == value {
                result = true;
                break;
            }
        }
    }

    return result;
}
