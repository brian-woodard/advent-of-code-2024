use std::fs;

enum State
{
    ReadMul,
    ReadOp1,
    ReadOp2,
}

fn main() {
    let input: Vec<u8> = fs::read("../../input.txt").expect("failed to open.");
    let mut value: u64 = 0;
    let mut op1: i32 = -1;
    let mut op2: i32 = -1;
    let mut idx: usize = 0;
    let mut state = State::ReadMul;
    let mut num_muls = 0;

    while idx < input.len() {
        match state {
            State::ReadMul => {
                if input[idx] == b'm' &&
                   input[idx+1] == b'u' &&
                   input[idx+2] == b'l' &&
                   input[idx+3] == b'(' {
                    idx += 4;
                    op1 = -1;
                    op2 = -1;
                    state = State::ReadOp1;
                } else {
                    idx += 1;
                }
            }
            State::ReadOp1 => {
                if input[idx] >= b'0' &&
                   input[idx] <= b'9' {
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
                if input[idx] >= b'0' &&
                   input[idx] <= b'9' {
                    if op2 == -1 {
                        op2 = (input[idx] - b'0') as i32;
                    } else {
                        op2 *= 10;
                        op2 += (input[idx] - b'0') as i32;
                    }
                    idx += 1;
                } else {
                    if input[idx] == b')' && op2 != -1 {
                        value += (op1 * op2) as u64;
                        num_muls += 1;
                    } else {
                        state = State::ReadMul;
                    }
                    idx += 1;
                }
            }
        }
    }

    println!("Value: {} {}", value, num_muls);
}
