use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file.");
    let reader = BufReader::new(file);
    let mut input: Vec<char> = Vec::new();
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut words_found: i32 = 0;

    for line in reader.lines() {
        if width == 0 {
            width = line.as_ref().unwrap().len() as i32;
        } else if width != line.as_ref().unwrap().len() as i32 {
            println!("Error: mismatched line lengths!");
            return;
        }

        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] as char);
        }
        height += 1;
    }

    println!("{} x {}", width, height);

    for y in 0..height {
        for x in 0..width {
            words_found += search(&input, x, y, width, height);
        }
    }

    println!("Number of words found: {}", words_found);
}

fn search(input: &Vec<char>, x: i32, y: i32, width: i32, height: i32) -> i32 {
    let s_idx: usize = ((y * width) + x) as usize;
    let mut result = 0;

    if input[s_idx] != 'X' {
        return 0;
    }

    // check left
    if x >= 3 {
        if input[s_idx-1] == 'M' &&
            input[s_idx-2] == 'A' &&
            input[s_idx-3] == 'S' {
            result += 1;
        }
    }

    // check right
    if x <= width-4 {
        if input[s_idx+1] == 'M' &&
            input[s_idx+2] == 'A' &&
            input[s_idx+3] == 'S' {
            result += 1;
        }
    }

    // check up
    if y >= 3 {
        let idx1 = s_idx - width as usize;
        let idx2 = idx1 - width as usize;
        let idx3 = idx2 - width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    // check down
    if y <= height-4 {
        let idx1 = s_idx + width as usize;
        let idx2 = idx1 + width as usize;
        let idx3 = idx2 + width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    // check upper left
    if x >= 3 && y >= 3 {
        let idx1 = s_idx - 1 - width as usize;
        let idx2 = idx1 - 1 - width as usize;
        let idx3 = idx2 - 1 - width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    // check upper right
    if x <= width-4 && y >= 3 {
        let idx1 = s_idx + 1 - width as usize;
        let idx2 = idx1 + 1 - width as usize;
        let idx3 = idx2 + 1 - width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    // check lower left
    if x >= 3 && y <= height-4 {
        let idx1 = s_idx - 1 + width as usize;
        let idx2 = idx1 - 1 + width as usize;
        let idx3 = idx2 - 1 + width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    // check lower right
    if x <= width-4 && y <= height-4 {
        let idx1 = s_idx + 1 + width as usize;
        let idx2 = idx1 + 1 + width as usize;
        let idx3 = idx2 + 1 + width as usize;
        if input[idx1] == 'M' &&
            input[idx2] == 'A' &&
            input[idx3] == 'S' {
            result += 1;
        }
    }

    return result
}
