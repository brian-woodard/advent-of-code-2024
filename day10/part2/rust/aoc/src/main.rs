use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let mut input: Vec<i32> = Vec::new();
    let mut trailheads: HashMap<usize, HashMap<usize, i32>> = HashMap::new();
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
            if let Some(_trailhead) = trailheads.get_mut(&i) {
                // do nothing
            } else {
                trailheads.insert(i, HashMap::new());
            }
            check_trailhead(&input, i as i32, -1, width, height, trailheads.get_mut(&i).unwrap());

            for trailhead in trailheads.get(&i) {
                for score in trailhead.values() {
                   //println!("{}: {:?}", i, score);
                   result += *score as u64;
                }
            }
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

fn check_trailhead(map: &Vec<i32>, idx: i32, prev_score: i32, width: i32, height: i32, trailhead: &mut HashMap<usize, i32>) {
    let x = idx % width;
    let y = idx / width;
    let mut score = map[idx as usize] as i32;

    if x < 0 || x >= width || y < 0 || y >= height {
        return;
    }

    if score != prev_score + 1 {
        return;
    }

    if score == 9 {
        if let Some(trailhead_score) = trailhead.get_mut(&(idx as usize)) {
            *trailhead_score += 1;
        } else {
            trailhead.insert(idx as usize, 1);
        }
    }

    // Check up
    if y > 0 {
        check_trailhead(map, idx - width, score, width, height, trailhead);
    }

    // Check down
    if y < height - 1 {
        check_trailhead(map, idx + width, score, width, height, trailhead);
    }

    // Check right
    if x < width - 1 {
        check_trailhead(map, idx + 1, score, width, height, trailhead);
    }

    // Check left
    if x > 0 {
        check_trailhead(map, idx - 1, score, width, height, trailhead);
    }
}
