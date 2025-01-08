use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::str::FromStr;

#[derive(Debug)]
struct Operation {
    value: u64,
    operands: Vec<i32>,
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
            operation.operands.push(i32::from_str(string).ok().unwrap());
        }

        input.operation.push(operation);
    }

    for operation in input.operation {
        result += check_operation(&operation);
    }

    println!("Result: {}", result);
}

fn check_operation(operation: &Operation) -> u64 {
    let mut op_mask: u64 = 0;
    let mask_size: u64 = operation.operands.len() as u64 - 1;
    let max_num_ops: u64 = 1 << mask_size;
    let mut result: u64 = 0;

    assert!(mask_size < 32);

    for _i in 0..max_num_ops {
        let mut value: u64 = operation.operands[0].try_into().unwrap();

        for ops in 0..mask_size {
            if (op_mask & (1 << ops)) == 0 {
                value += operation.operands[ops as usize + 1] as u64;
            } else {
                value *= operation.operands[ops as usize + 1] as u64;
            }
        }

        if value == operation.value {
            result = value;
            break;
        }

        op_mask += 1;
    }

    return result;
}
