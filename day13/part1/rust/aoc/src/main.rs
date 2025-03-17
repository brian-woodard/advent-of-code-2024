use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

#[derive(Debug, Copy, Clone)]
struct Equation {
    coefficient_1: i32,
    coefficient_2: i32,
    value: i32,
}

#[derive(Debug, Copy, Clone)]
struct Equations {
    equation: [Equation; 2],
    button_a: i32,
    button_b: i32,
}

fn main() {
    // let file = File::open("../../test.txt").expect("failed to open"); // 480
    let file = File::open("../../input.txt").expect("failed to open");
    let reader = BufReader::new(file);
    let mut machines : Vec<Equations> = Vec::new();
    let mut eq = Equations {
        equation: [ Equation {
            coefficient_1: 0,
            coefficient_2: 0,
            value: 0,
        }; 2],
        button_a: 0,
        button_b: 0,
    };
    let mut cost: u64 = 0;

    for line in reader.lines() {

        if line.as_ref().unwrap().starts_with("Button A") {
            let tokens: Vec<&str> = line.as_ref().unwrap().split("+").collect();
            let tokens_2: Vec<&str> = tokens[1].split(",").collect();

            assert!(tokens.len() == 3);

            eq.equation[0].coefficient_1 = tokens_2[0].parse().unwrap();
            eq.equation[1].coefficient_1 = tokens[2].parse().unwrap();
        } else if line.as_ref().unwrap().starts_with("Button B") {
            let tokens: Vec<&str> = line.as_ref().unwrap().split("+").collect();
            let tokens_2: Vec<&str> = tokens[1].split(",").collect();

            assert!(tokens.len() == 3);

            eq.equation[0].coefficient_2 = tokens_2[0].parse().unwrap();
            eq.equation[1].coefficient_2 = tokens[2].parse().unwrap();
        } else if line.as_ref().unwrap().starts_with("Prize") {
            let tokens: Vec<&str> = line.as_ref().unwrap().split("=").collect();
            let tokens_2: Vec<&str> = tokens[1].split(",").collect();

            assert!(tokens.len() == 3);

            eq.equation[0].value = tokens_2[0].parse().unwrap();
            eq.equation[1].value = tokens[2].parse().unwrap();

            machines.push(eq);
        }
    }

    println!("Number of machines: {}", machines.len());

    for mut machine in machines.iter_mut() {
        solve_equation(&mut machine);
        cost += (machine.button_a as u64 * 3) + machine.button_b as u64;
    }

    println!("Cost: {}", cost);
}

fn solve_equation(equations: &mut Equations) {
    let numerator = (equations.equation[0].value * equations.equation[1].coefficient_2) - (equations.equation[1].value * equations.equation[0].coefficient_2);
    let denominator = (equations.equation[0].coefficient_1 * equations.equation[1].coefficient_2) - (equations.equation[1].coefficient_1 * equations.equation[0].coefficient_2);

    if numerator % denominator == 0 {
        let x = numerator / denominator;
        let y = (equations.equation[0].value - (equations.equation[0].coefficient_1 * x)) / equations.equation[0].coefficient_2;

        equations.button_a = x;
        equations.button_b = y;
    }
}
