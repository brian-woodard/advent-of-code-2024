use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

#[derive(PartialEq)]
enum State {
    MoveUp,
    MoveRight,
    MoveDown,
    MoveLeft,
    Finished,
}

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file.");
    let reader = BufReader::new(file);
    let mut room: Vec<char> = Vec::new();
    let mut width: i32 = 0;
    let mut height: i32 = 0;
    let mut start_idx: i32 = 0;
    let mut curr_idx: i32 = 0;
    let mut moves = 0;
    let mut state = State::Finished;
    let mut idx: i32 = 0;
    let mut loop_locations: HashMap<i32, bool> = HashMap::new();
    let mut num_loop_locations: i32 = 0;

    for line in reader.lines() {
        if width == 0 {
            width = line.as_ref().unwrap().len() as i32;
        } else if width != line.as_ref().unwrap().len() as i32 {
            println!("Error: mismatched line widths");
            return;
        }

        for i in 0..line.as_ref().unwrap().len() {

            if line.as_ref().unwrap().as_bytes()[i] == b'^' && state == State::Finished {
                start_idx = idx;
                state = State::MoveUp;
            }

            room.push(line.as_ref().unwrap().as_bytes()[i] as char);

            idx += 1;
        }

        height += 1;
    }

    println!("{} x {}", width, height);
    println!("start_idx = {}", start_idx);
    assert!(room.len() == (width * height).try_into().unwrap());
    print_room(&room, width, height);

    curr_idx = start_idx;
    while state != State::Finished {
        if move_guard(&mut room, width, height, &mut curr_idx, &mut state) {
            if curr_idx >= 0 && curr_idx < room.len().try_into().unwrap() {
                room[curr_idx as usize] = 'o';
            }
            moves += 1;

            if state != State::Finished {
                if let Some(location) = loop_locations.get(&curr_idx) {
                    // location has already been checked for loop, do nothing
                } else {
                    loop_locations.insert(curr_idx, check_for_loop(&mut room, width, height, curr_idx, start_idx));
                }
            }
        }
    }

    room[start_idx as usize] = '*';
    for (idx, loop_found) in loop_locations {
        if loop_found {
            room[idx as usize] = 'O';
            num_loop_locations += 1;
        }
    }
    print_room(&room, width, height);

    println!("Result: {} {}", moves, num_loop_locations);
}

fn print_room(room: &Vec<char>, width: i32, height: i32) {
    println!("\nRoom:");
    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;
            print!("{}", room[idx as usize]);
        }
        println!("");
    }
}

fn move_guard(room: &mut Vec<char>, width: i32, height: i32, idx: &mut i32, state: &mut State) -> bool {
    let mut x = *idx % width;
    let mut y = *idx / width;
    let mut result = true;

    match state {
        State::MoveUp => {
            // move up
            y -= 1;

            // new index
            *idx = (y * width) + x;

            // test square
            if y < 0 {
                *state = State::Finished;
            } else if room[*idx as usize] == '#' {
                y += 1;
                *idx = (y * width) + x;
                *state = State::MoveRight;
                result = false;
            }
        }
        State::MoveRight => {
            // move right 
            x += 1;

            // new index
            *idx = (y * width) + x;

            // test square
            if x >= width {
                *state = State::Finished;
            } else if room[*idx as usize] == '#' {
                x -= 1;
                *idx = (y * width) + x;
                *state = State::MoveDown;
                result = false;
            }
        }
        State::MoveDown => {
            // move down 
            y += 1;

            // new index
            *idx = (y * width) + x;

            // test square
            if y >= height {
                *state = State::Finished;
            } else if room[*idx as usize] == '#' {
                y -= 1;
                *idx = (y * width) + x;
                *state = State::MoveLeft;
                result = false;
            }
        }
        State::MoveLeft => {
            // move left 
            x -= 1;

            // new index
            *idx = (y * width) + x;

            // test square
            if x < 0 {
                *state = State::Finished;
            } else if room[*idx as usize] == '#' {
                x += 1;
                *idx = (y * width) + x;
                *state = State::MoveUp;
                result = false;
            }
        }
        _ => {
            *state = State::Finished;
        }
    }

    return result;
}

fn check_for_loop(room: &mut Vec<char>, width: i32, height: i32, curr_idx: i32, start_idx: i32) -> bool {
    // add a barrier at current location
    let prev = room[curr_idx as usize];
    room[curr_idx as usize] = '#';

    // see if we're in a loop
    let mut moves = 0;
    let mut state = State::MoveUp;
    let mut start_idx_loop = start_idx;
    let mut start_idx_loop_check = start_idx;
    let mut visited: HashMap<i32, bool> = HashMap::new();

    visited.insert(start_idx_loop, true);

    while state != State::Finished {
        let able_to_move = move_guard(room, width, height, &mut start_idx_loop, &mut state);
        moves += 1;

        //if able_to_move && state != State::Finished {
        //    if let Some(check_visit) = visited.get(&start_idx_loop_check) {
        //        if start_idx_loop_check == start_idx_loop {
        //            break;
        //        }
        //    } else {
        //        // update this is our new start index
        //        start_idx_loop_check = start_idx_loop;
        //        visited.insert(start_idx_loop, true);
        //    }
        //}

        if moves > (width * height * 10) {
            break;
        }
    }

    // remove barrier at current location
    room[curr_idx as usize] = prev;

    return state != State::Finished;
}
