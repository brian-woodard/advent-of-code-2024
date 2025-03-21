use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

#[derive(Debug)]
struct Vector {
    x: i32,
    y: i32,
}

#[derive(Debug)]
struct Robot {
    position: Vector,
    velocity: Vector,
}

fn main() {
    //let file = File::open("../../test.txt").expect("file not opened");
    let file = File::open("../../input.txt").expect("file not opened");
    let reader = BufReader::new(file);
    let mut robots: Vec<Robot> = Vec::new();
    let mut map: Vec<char> = Vec::new();
    let mut robot_counts: HashMap<i32, i32> = HashMap::new();
    //let width: i32 = 11;
    //let height: i32 = 7;
    let width: i32 = 101;
    let height: i32 = 103;

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
    robot_counts.clear();
    for r in &robots {
        let idx = (r.position.y * width) + r.position.x;

        if let Some(pos) = robot_counts.get_mut(&idx) {
            *pos += 1;
        } else {
            robot_counts.insert(idx, 1);
        }
    }

    print_map(&map, &robot_counts, width, height);

    // simulate robot movements for 100 seconds
    for i in 0..100 {
        for r in robots.iter_mut() {
            r.position.x += r.velocity.x;
            r.position.y += r.velocity.y;

            if r.position.x < 0 {
                r.position.x += width;
            }

            if r.position.x >= width {
                r.position.x -= width;
            }

            if r.position.y < 0 {
                r.position.y += height;
            }

            if r.position.y >= height {
                r.position.y -= height;
            }
        }
    }

    // generate map of robot counts after simulation
    robot_counts.clear();
    for r in robots {
        let idx = (r.position.y * width) + r.position.x;

        if let Some(pos) = robot_counts.get_mut(&idx) {
            *pos += 1;
        } else {
            robot_counts.insert(idx, 1);
        }
    }

    print_map(&map, &robot_counts, width, height);

    // quad 1 count
    let mut count_1 = 0;
    for y in 0..height/2 {
        for x in 0..width/2 {
            let idx = (y * width) + x;
            if let Some(pos) = robot_counts.get(&idx) {
                count_1 += *pos;
            }
        }
    }
    println!("Quad 1: {}", count_1);

    // quad 2 count
    let mut count_2 = 0;
    for y in 0..height/2 {
        for x in width/2 + 1..width {
            let idx = (y * width) + x;
            if let Some(pos) = robot_counts.get(&idx) {
                count_2 += *pos;
            }
        }
    }
    println!("Quad 2: {}", count_2);

    // quad 3 count
    let mut count_3 = 0;
    for y in height/2 + 1..height {
        for x in 0..width/2 {
            let idx = (y * width) + x;
            if let Some(pos) = robot_counts.get(&idx) {
                count_3 += *pos;
            }
        }
    }
    println!("Quad 3: {}", count_3);

    // quad 4 count
    let mut count_4 = 0;
    for y in height/2 + 1..height {
        for x in width/2 + 1..width {
            let idx = (y * width) + x;
            if let Some(pos) = robot_counts.get(&idx) {
                count_4 += *pos;
            }
        }
    }
    println!("Quad 4: {}", count_4);

    println!("Safety score: {}", (count_1 * count_2 * count_3 * count_4));
}

fn print_map(map: &Vec<char>, robot_counts: &HashMap<i32, i32>, width: i32, height: i32) {
    println!();
    for y in 0..height {
        for x in 0..width {
            let idx = (y * width) + x;
            if let Some(pos) = robot_counts.get(&idx) {
                print!("{}", *pos);
            } else {
                print!(".");
            }
        }
        println!();
    }
    println!();
}
