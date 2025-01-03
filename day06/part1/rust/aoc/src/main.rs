use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

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
    let mut unique_squares = 0;
    let mut state = State::Finished;
    let mut idx: i32 = 0;

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
    //print_room(&room, width, height);

    while state != State::Finished {
        if move_guard(&mut room, width, height, &mut start_idx, &mut state) {
            unique_squares += 1;
        }
        //print_room(&room, width, height);
    }

    println!("Result: {}", unique_squares);
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
    let result = room[*idx as usize] != 'X';

    match state {
        State::MoveUp => {
            room[*idx as usize] = 'X';

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
            }
        }
        State::MoveRight => {
            room[*idx as usize] = 'X';

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
            }
        }
        State::MoveDown => {
            room[*idx as usize] = 'X';

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
            }
        }
        State::MoveLeft => {
            room[*idx as usize] = 'X';

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
            }
        }
        _ => {
            *state = State::Finished;
        }
    }

    return result;
}
