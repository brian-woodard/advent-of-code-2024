use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

fn main() {
    //let file = File::open("../../test1.txt").expect("failed to open file"); // 2028
    //let file = File::open("../../test2.txt").expect("failed to open file"); // 10092
    let file = File::open("../../input.txt").expect("failed to open file"); // 1294459
    let reader = BufReader::new(file);
    let mut map: Vec<char> = Vec::new();
    let mut moves: Vec<char> = Vec::new();
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut start: i32 = 0;
    let mut read_map = true;
    let mut gps_sum: u64 = 0;

    for line in reader.lines() {
        if read_map {
            if line.as_ref().unwrap().len() > 0 {
                if width == 0 {
                    width = line.as_ref().unwrap().len() as i32;
                } else {
                    assert!(width == line.as_ref().unwrap().len() as i32);
                }

                for i in 0..line.as_ref().unwrap().len() {
                    map.push(line.as_ref().unwrap().as_bytes()[i] as char);
                    if map[map.len()-1] == '@' {
                        start = map.len() as i32 - 1;
                    }
                }

                height += 1;
            } else {
                read_map = false;
            }
        } else {
            for i in 0..line.as_ref().unwrap().len() {
                moves.push(line.as_ref().unwrap().as_bytes()[i] as char);
            }
        }
    }

    println!("{} x {}", width, height);
    println!("Number of moves: {}", moves.len());

    print_map(&map, width);

    for i in 0..moves.len() {
        do_move(&mut map, moves[i], &mut start, width, height);
        //print_map(&map, width);
    }

    print_map(&map, width);

    // calculate gps sum
    for i in 0..map.len() {
        if map[i] == 'O' {
            let x = i as i32 % width;
            let y = i as i32 / width;

            gps_sum += (100 * y as u64) + x as u64;
        }
    }

    println!("GPS sum: {}", gps_sum);
}

fn print_map(map: &Vec<char>, width: i32) {
    println!();
    for i in 0..map.len() {
        if i > 0 && i as i32 % width == 0 {
            println!();
        }
        print!("{}", map[i]);
    }
    println!();
}

fn do_move(map: &mut Vec<char>, move_cmd: char, position: &mut i32, width: i32, height: i32) -> bool {
    let x = *position % width;
    let y = *position / width;

    match move_cmd {
        '<' => {
            if x > 1 {
                let mut idx = (y * width) + x - 1;
                if map[idx as usize] == '.' {
                    let mut update_position = false;
                    if map[*position as usize] == '@' {
                        update_position = true;
                    }
                    map[idx as usize] = map[*position as usize];
                    map[*position as usize] = '.';
                    if update_position {
                        *position = idx;
                    }
                    return true;
                } else if map[idx as usize] == 'O' {
                    if do_move(map, move_cmd, &mut idx, width, height) {
                        let mut update_position = false;
                        if map[*position as usize] == '@' {
                            update_position = true;
                        }
                        map[idx as usize] = map[*position as usize];
                        map[*position as usize] = '.';
                        if update_position {
                            *position = idx;
                        }
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        },
        '>' => {
            if x < width - 1 {
                let mut idx = (y * width) + x + 1;
                if map[idx as usize] == '.' {
                    let mut update_position = false;
                    if map[*position as usize] == '@' {
                        update_position = true;
                    }
                    map[idx as usize] = map[*position as usize];
                    map[*position as usize] = '.';
                    if update_position {
                        *position = idx;
                    }
                    return true;
                } else if map[idx as usize] == 'O' {
                    if do_move(map, move_cmd, &mut idx, width, height) {
                        let mut update_position = false;
                        if map[*position as usize] == '@' {
                            update_position = true;
                        }
                        map[idx as usize] = map[*position as usize];
                        map[*position as usize] = '.';
                        if update_position {
                            *position = idx;
                        }
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        },
        '^' => {
            if y > 1 {
                let mut idx = ((y - 1) * width) + x;
                if map[idx as usize] == '.' {
                    let mut update_position = false;
                    if map[*position as usize] == '@' {
                        update_position = true;
                    }
                    map[idx as usize] = map[*position as usize];
                    map[*position as usize] = '.';
                    if update_position {
                        *position = idx;
                    }
                    return true;
                } else if map[idx as usize] == 'O' {
                    if do_move(map, move_cmd, &mut idx, width, height) {
                        let mut update_position = false;
                        if map[*position as usize] == '@' {
                            update_position = true;
                        }
                        map[idx as usize] = map[*position as usize];
                        map[*position as usize] = '.';
                        if update_position {
                            *position = idx;
                        }
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        },
        'v' => {
            if y < height - 1 {
                let mut idx = ((y + 1) * width) + x;
                if map[idx as usize] == '.' {
                    let mut update_position = false;
                    if map[*position as usize] == '@' {
                        update_position = true;
                    }
                    map[idx as usize] = map[*position as usize];
                    map[*position as usize] = '.';
                    if update_position {
                        *position = idx;
                    }
                    return true;
                } else if map[idx as usize] == 'O' {
                    if do_move(map, move_cmd, &mut idx, width, height) {
                        let mut update_position = false;
                        if map[*position as usize] == '@' {
                            update_position = true;
                        }
                        map[idx as usize] = map[*position as usize];
                        map[*position as usize] = '.';
                        if update_position {
                            *position = idx;
                        }
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        },
        _ => {},
    };

    return false;
}
