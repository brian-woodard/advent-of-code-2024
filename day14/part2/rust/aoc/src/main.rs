use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

#[derive(Debug, Clone, Copy)]
struct Vector {
    x: i32,
    y: i32,
}

#[derive(Debug, Clone, Copy)]
struct Robot {
    position: Vector,
    velocity: Vector,
}

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let width: i32 = 101;
    let height: i32 = 103;
    let mut robots: Vec<Robot> = Vec::new();
    let mut map: Vec<char> = Vec::new();
    let mut robot_counts: HashMap<i32, i32> = HashMap::new();

    map.reserve((width * height).try_into().unwrap());

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

    // generate initial map of robot counts
    for robot in robots.iter() {
        let idx = (robot.position.y * width) + robot.position.x;

        if let Some(robot_count) = robot_counts.get_mut(&idx) {
            *robot_count += 1;
        } else {
            robot_counts.insert(idx, 1);
        }
    }

    for i in 0..7371 {
        for robot in robots.iter_mut() {
            robot.position.x += robot.velocity.x;
            robot.position.y += robot.velocity.y;

            if robot.position.x < 0 {
                robot.position.x += width;
            }

            if robot.position.x >= width {
                robot.position.x -= width;
            }

            if robot.position.y < 0 {
                robot.position.y += height;
            }

            if robot.position.y >= height {
                robot.position.y -= height;
            }
        }
    }

    // generate initial map of robot counts
    robot_counts.clear();
    for robot in robots.iter() {
        let idx = (robot.position.y * width) + robot.position.x;

        if let Some(robot_count) = robot_counts.get_mut(&idx) {
            *robot_count += 1;
        } else {
            robot_counts.insert(idx, 1);
        }
    }

    print_map(&map, &robot_counts, width, height);
}

fn print_map(map: &Vec<char>, robot_counts: &HashMap<i32, i32>, width: i32, height: i32) {
    println!();
    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;

            if let Some(robot_count) = robot_counts.get(&idx) {
                print!("{}", robot_count);
            } else {
                print!(".");
            }
        }
        println!();
    }
    println!();
}
