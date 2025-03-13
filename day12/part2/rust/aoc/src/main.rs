use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
struct Subregion {
    area: i32,
    perimeter: i32,
    sides: i32,
    cost: i32,
}

#[derive(Debug, PartialEq)]
struct Region {
    map: Vec<char>,
    subregions: Vec<Subregion>,
    width: i32,
    height: i32,
}

fn main() {
    //let file = File::open("../../test1.txt").expect("failed to open file"); // 80
    //let file = File::open("../../test2.txt").expect("failed to open file"); // 436
    //let file = File::open("../../test3.txt").expect("failed to open file"); // 1206
    //let file = File::open("../../test4.txt").expect("failed to open file"); // 68
    //let file = File::open("../../test5.txt").expect("failed to open file"); // 236
    //let file = File::open("../../test6.txt").expect("failed to open file"); // 368
    let file = File::open("../../input.txt").expect("failed to open file");
    let reader = BufReader::new(file);
    let mut region = Region {
        map: Vec::new(),
        subregions: Vec::new(),
        width: 0,
        height: 0,
    };
    let mut regions: HashMap<char, Region> = HashMap::new();
    let mut cost: u64 = 0;

    for line in reader.lines() {
        if region.width == 0 {
            region.width = line.as_ref().unwrap().len() as i32;
        } else {
            assert!(region.width == line.as_ref().unwrap().len() as i32);
        }

        for i in 0..line.as_ref().unwrap().len() {
            region.map.push(line.as_ref().unwrap().as_bytes()[i] as char);
        }

        region.height += 1;
    }

    println!("{} x {}", region.width, region.height);

    //print_region(&region);

    for i in 0..region.map.len() {
        let reg = regions.get_mut(&region.map[i]);

        if reg == None {
            let mut new_region = Region {
                map: Vec::new(),
                subregions: Vec::new(),
                width: region.width,
                height: region.height,
            };

            new_region.map.reserve(region.map.len());
            for _j in 0..region.map.len() {
                new_region.map.push('.');
            }

            new_region.map[i] = region.map[i];
            regions.insert(region.map[i], new_region);
        } else {
            reg.unwrap().map[i] = region.map[i];
        }
    }

    println!("Regions: {}", regions.len());

    for (_key, mut val) in regions {
        //println!("{}:", key);
        //print_region(&val);

        calc_area_and_perimeter(&mut val);

        //print_region(&val);

        for subregion in val.subregions {
            cost += subregion.cost as u64;
        }
    }

    println!("Total cost: {}", cost);
}

#[allow(dead_code)]
fn print_region(region: &Region) {
    println!();
    for y in 0..region.height {
        for x in 0..region.width {
            let idx = (y * region.width) + x;
            print!("{}", region.map[idx as usize]);
        }
        println!();
    }
    println!();
}

fn calc_area_and_perimeter(region: &mut Region) {
    loop {
        let mut subregion = Subregion {
            area: 0,
            perimeter: 0,
            sides: 0,
            cost: 0,
        };
        let mut idx: i32 = -1;

        for i in 0..region.map.len() {
            if region.map[i] != '.' && region.map[i] != 'x' {
                calc_area_and_perimeter_recursive(region, &mut subregion, i);

                subregion.cost = subregion.area * subregion.sides;
                region.subregions.push(subregion);

                idx = i as i32;
                break;
            }
        }

        if idx == -1 {
            break;
        }
    }
}

fn calc_area_and_perimeter_recursive(region: &mut Region, subregion: &mut Subregion, idx: usize)
{
    let w = region.width;
    let x = idx as i32 % region.width;
    let y = idx as i32 / region.width;
    let value = region.map[idx];

    region.map[idx] = 'x';
    subregion.area += 1;

    // Check left
    if x - 1 >= 0 {
        if region.map[idx - 1] == value {
            calc_area_and_perimeter_recursive(region, subregion, idx - 1);
        } else if region.map[idx - 1] == '.' {
            if y - 1 < 0 || region.map[idx - w as usize] == '.' || (region.map[idx - w as usize] != '.' && region.map[idx - w as usize - 1] != '.') {
                subregion.sides += 1;
            }
            subregion.perimeter += 1;
        }
    } else {
        if y - 1 < 0 || region.map[idx - w as usize] == '.' {
            subregion.sides += 1;
        }
        subregion.perimeter += 1;
    }

    // Check right
    if x + 1 < region.width {
        if region.map[idx + 1] == value {
            calc_area_and_perimeter_recursive(region, subregion, idx + 1);
        } else if region.map[idx + 1] == '.' {
            if y - 1 < 0 || region.map[idx - w as usize] == '.' || (region.map[idx - w as usize] != '.' && region.map[idx - w as usize + 1] != '.') {
                subregion.sides += 1;
            }
            subregion.perimeter += 1;
        }
    } else {
        if y - 1 < 0 || region.map[idx - w as usize] == '.' {
            subregion.sides += 1;
        }
        subregion.perimeter += 1;
    }

    // Check up
    if y - 1 >= 0 {
        if region.map[idx - w as usize] == value {
            calc_area_and_perimeter_recursive(region, subregion, idx - w as usize);
        } else if region.map[idx - w as usize] == '.' {
            if x - 1 < 0 || (region.map[idx - 1] == '.' || region.map[idx - w as usize - 1] != '.') {
                subregion.sides += 1;
            }
            subregion.perimeter += 1;
        }
    } else {
        if x - 1 < 0 || region.map[idx - 1] == '.' {
            subregion.sides += 1;
        }
        subregion.perimeter += 1;
    }

    // Check down
    if y + 1 < region.height {
        if region.map[idx + w as usize] == value {
            calc_area_and_perimeter_recursive(region, subregion, idx + w as usize);
        } else if region.map[idx + w as usize] == '.' {
            if x - 1 < 0 || (region.map[idx - 1] == '.' || region.map[idx + w as usize - 1] != '.') {
                subregion.sides += 1;
            }
            subregion.perimeter += 1;
        }
    } else {
        if x - 1 < 0 || region.map[idx - 1] == '.' {
            subregion.sides += 1;
        }
        subregion.perimeter += 1;
    }
}
