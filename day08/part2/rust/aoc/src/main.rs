use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut input: Vec<char> = Vec::new();
    let mut nodes: HashMap<char, Vec<i32>> = HashMap::new();
    let mut total_antinodes_on_map: HashMap<i32, bool> = HashMap::new();

    for line in reader.lines() {
        if width == 0 {
            width = line.as_ref().unwrap().len().try_into().unwrap();
        } else {
            assert!(width == line.as_ref().unwrap().len().try_into().unwrap());
        }

        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] as char);
        }

        height += 1;
    }

    println!("{} x {}", width, height);

    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;
            if input[idx as usize] != '.' {
                let mut node_vec = nodes.get_mut(&input[idx as usize]);

                if node_vec.is_none() {
                    nodes.insert(input[idx as usize], Vec::new());
                    node_vec = nodes.get_mut(&input[idx as usize]);
                }

                if node_vec.is_some() {
                    node_vec.as_mut().unwrap().push(idx);

                    if node_vec.as_ref().unwrap().len() > 1 {
                        let n = node_vec.as_ref().unwrap().len() - 1;
                        for i in 0..n {
                            find_antinodes(&mut total_antinodes_on_map,
                                           node_vec.as_ref().unwrap()[i],
                                           node_vec.as_ref().unwrap()[n],
                                           width,
                                           height);
                        }
                    }
                }
            }
        }
    }

    println!("Result: {}", total_antinodes_on_map.len());
}

fn find_antinodes(antinodes: &mut HashMap<i32, bool>, idx1: i32, idx2: i32, width: i32, height: i32) {
    let x1 = idx1 % width;
    let y1 = idx1 / width;
    let x2 = idx2 % width;
    let y2 = idx2 / width;

    let dist_x = x2 - x1;
    let dist_y = y2 - y1;

    let mut new_x1 = x1 - dist_x;
    let mut new_y1 = y1 - dist_y;
    let mut new_x2 = x2 + dist_x;
    let mut new_y2 = y2 + dist_y;

    antinodes.insert(idx1, true);
    antinodes.insert(idx2, true);

    while new_x1 >= 0 && new_x1 < width &&
        new_y1 >= 0 && new_y1 < height {
        let idx = (new_y1 * width) + new_x1;
        antinodes.insert(idx, true);

        new_x1 -= dist_x;
        new_y1 -= dist_y;
    }

    while new_x2 >= 0 && new_x2 < width &&
        new_y2 >= 0 && new_y2 < height {
        let idx = (new_y2 * width) + new_x2;
        antinodes.insert(idx, true);

        new_x2 += dist_x;
        new_y2 += dist_y;
    }
}
