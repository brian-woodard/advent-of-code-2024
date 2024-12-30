use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

fn main() {
    let file = File::open("../../input.txt").expect("file not found.");
    let reader = BufReader::new(file);
    let mut input: Vec<Vec<i32>> = Vec::new();
    let mut num_safe: u64 = 0;

    for line in reader.lines() {
        let nums : Vec<i32> = line
            .unwrap()
            .split(" ")
            .filter_map(|num| i32::from_str(num).ok())
            .collect();

        input.push(nums);
    }

    for report in input {
        let decreasing = report[1] < report[0];
        let mut safe = true;

        for i in 0..report.len()-1 {
            let delta = report[i+1] - report[i];

            if delta == 0 {
                safe = false;
                break;
            }

            if decreasing {
                if delta > -1 || delta < -3 {
                    safe = false;
                    break;
                }
            }

            if !decreasing {
                if delta < 1 || delta > 3 {
                    safe = false;
                    break;
                }
            }
        }

        if safe {
            num_safe += 1;
        }
    }

    println!("Number of safe reports: {}", num_safe);
}
