use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

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
            if let Some(trailhead) = trailheads.get_mut(&i) {
                // do nothing
            } else {
                trailheads.insert(i, HashMap::new());
            }
            check_trailhead(&input, i as i32, -1, width, height, trailheads.get_mut(&i).unwrap());
            result += trailheads.get(&i).unwrap().len() as u64;
            println!("Result: ({}, {}) {}", i % width as usize, i / width as usize, trailheads.get(&i).unwrap().len());
        }
    }

    println!("Result: {}", result);
}

fn check_trailhead(map: &Vec<u8>, idx: i32, prev_score: i32, width: i32, height: i32, trailhead_score: &mut HashMap<usize, bool>)
{
    let x = idx as i32 % width;
    let y = idx as i32 / width;
    let mut score = 0;

    if x < 0 || x >= width || y < 0 || y >= height {
        return;
    }

    score = map[idx as usize] as i32;

    if score != prev_score + 1 {
        return;
    }

    if score == 9 {
       trailhead_score.insert(idx as usize, true);
       return;
    }

    // Check up
    if y > 0 {
        check_trailhead(map, idx - width, score, width, height, trailhead_score);
    }

    // Check down
    if y < height-1 {
        check_trailhead(map, idx + width, score, width, height, trailhead_score);
    }

    // Check right
    if y < width-1 {
        check_trailhead(map, idx + 1, score, width, height, trailhead_score);
    }

    // Check left
    if x > 0 {
        check_trailhead(map, idx - 1, score, width, height, trailhead_score);
    }

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
