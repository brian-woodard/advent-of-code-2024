use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

struct Vector {
    x: i32,
    y: i32,
}

struct Robot {
    position: Vector,
    velocity: Vector,
}

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let width = 101;
    let height = 103;
    let mut robots: Vec<Robot> = Vec::new();
    let mut map: Vec<char> = Vec::new();

    map.reserve(width * height);

    for line in reader.lines() {
        let tokens: Vec<&str> = line.as_ref().unwrap()
            .split(|c: char| c == '=' || c == ',' || c.is_ascii_whitespace())
            .filter(|p| !p.is_empty())
            .collect();

        assert!(tokens.len() == 6);

        let robot = Robot {
            position: Vector {
                x: tokens[1].parse().unwrap(),
                y: tokens[2].parse().unwrap(),
            },
            velocity: Vector {
                x: tokens[4].parse().unwrap(),
                y: tokens[5].parse().unwrap(),
            },
        };

        robots.push(robot);
    }

    println!("Number of robots: {}", robots.len());
}
