use std::fs;

enum State
{
    ReadMul,
    ReadOp1,
    ReadOp2,
}

fn main() {
    let input = fs::read("../../input.txt").expect("failed to read file.");
    let mut value: u64 = 0;
    let mut op1: i32 = -1;
    let mut op2: i32 = -1;
    let mut enabled = true;
    let mut state = State::ReadMul;
    let mut idx: usize = 0;

    while idx < input.len() {
        match state {
            State::ReadMul => {
                if enabled && input[idx] == b'm' &&
                    input[idx+1] == b'u' &&
                    input[idx+2] == b'l' &&
                    input[idx+3] == b'(' {
                    idx += 4;

                    op1 = -1;
                    op2 = -1;
                    state = State::ReadOp1;
                } else if input[idx] == b'd' &&
                    input[idx+1] == b'o' &&
                    input[idx+2] == b'(' &&
                    input[idx+3] == b')' {
                    idx += 4;
                    enabled = true;
                } else if input[idx] == b'd' &&
                    input[idx+1] == b'o' &&
                    input[idx+2] == b'n' &&
                    input[idx+3] == b'\'' &&
                    input[idx+4] == b't' &&
                    input[idx+5] == b'(' &&
                    input[idx+6] == b')' {
                    idx += 7;
                    enabled = false;
                } else {
                    idx += 1;
                }
            }
            State::ReadOp1 => {
                if input[idx] >= b'0' && input[idx] <= b'9' {
                    if op1 == -1 {
                        op1 = (input[idx] - b'0') as i32;
                    } else {
                        op1 *= 10;
                        op1 += (input[idx] - b'0') as i32;
                    }
                    idx += 1;
                } else {
                    if input[idx] == b',' && op1 != -1 {
                        state = State::ReadOp2;
                    } else {
                        state = State::ReadMul;
                    }
                    idx += 1;
                }
            }
            State::ReadOp2 => {
                if input[idx] >= b'0' && input[idx] <= b'9' {
                    if op2 == -1 {
                        op2 = (input[idx] - b'0') as i32;
                    } else {
                        op2 *= 10;
                        op2 += (input[idx] - b'0') as i32;
                    }
                    idx += 1;
                } else {
                    if input[idx] == b')' && op2 != -1 {
                        if enabled {
                            value += (op1 * op2) as u64;
                        }
                    }
                    state = State::ReadMul;
                    idx += 1;
                }
            }
        }
    }

    println!("Value: {}", value);
}
