use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

#[derive(Debug)]
enum Op {
    Concate,
    Addition,
    Multiply,
    NumOps,
}

impl From<usize> for Op {
    fn from(value: usize) -> Self {
        match value {
            0 => Op::Concate,
            1 => Op::Addition,
            2 => Op::Multiply,
            _ => Op::NumOps,
        }
    }
}

#[derive(Debug)]
struct Operand {
    value: i32,
    op: Op,
}

#[derive(Debug)]
struct Operation {
    value: u64,
    operands: Vec<Operand>,
}

#[derive(Debug)]
struct Input {
    operation: Vec<Operation>,
}

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let mut result: u64 = 0;
    let mut input: Input = Input {
        operation: Vec::new()
    };

    for line in reader.lines() {
        let mut operation: Operation = Operation {
            value: 0,
            operands: Vec::new()
        };

        let parsed_strings: Vec<&str> = line
            .as_ref()
            .unwrap()
            .split(": ")
            .collect();

        if parsed_strings.len() != 2 {
            println!("Error parsing");
            return;
        }

        operation.value = u64::from_str(parsed_strings[0]).ok().unwrap();

        let parsed_operands: Vec<&str> = parsed_strings[1]
            .split(" ")
            .collect();

        for string in parsed_operands {
            operation.operands.push( Operand {
                value: i32::from_str(string).ok().unwrap(),
                op: Op::Concate
            });
        }

        input.operation.push(operation);
    }

    for operation in input.operation.iter_mut() {
        result += check_operation(operation);
    }

    println!("Result: {}", result);
}

fn check_operation(operation: &mut Operation) -> u64 {
    let operand_count = operation.operands.len() - 1;
    let mut loop_count: usize = 1;
    let mut result: u64 = 0;

    assert!(operand_count > 0 && operand_count < 32);

    for _i in 0..operand_count {
        loop_count *= Op::NumOps as usize;
    }

    for i in 0..loop_count {
        let mut divisor = 1;

        for j in 0..operand_count {
            operation.operands[j].op = ((i / divisor) % Op::NumOps as usize).try_into().unwrap();
            divisor *= 3;
        }

        let mut value: u64 = operation.operands[0].value.try_into().unwrap();

        for j in 0..operand_count {
            match operation.operands[j].op {
                Op::Concate => {
                    let mut tmp = operation.operands[j+1].value;
                    while tmp > 0 {
                        tmp /= 10;
                        value *= 10;
                    }
                    value += operation.operands[j+1].value as u64;
                }
                Op::Addition => {
                    value += operation.operands[j+1].value as u64;
                }
                Op::Multiply => {
                    value *= operation.operands[j+1].value as u64;
                }
                Op::NumOps => {
                    // do nothing
                }
            }
        }

        if value == operation.value {
            result = value;
            break;
        }
    }

    return result;
}
