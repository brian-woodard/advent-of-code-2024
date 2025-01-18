use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

#[derive(Debug)]
struct DiskNode {
    file_id: i32,
}

fn main() {
    let file = File::open("../../input.txt").expect("failed to open file.");
    let reader = BufReader::new(file);
    let mut input: Vec<char> = Vec::new();
    let mut disk: Vec<DiskNode> = Vec::new();

    for line in reader.lines() {
        for i in 0..line.as_ref().unwrap().len() {
            input.push(line.as_ref().unwrap().as_bytes()[i] as char);
        }
    }

    disk.reserve(input.len());

    let mut is_file = true;
    let mut file_id: i32 = 0;
    for i in 0..input.len() {
        let count = input[i] as usize - b'0' as usize;

        for _j in 0..count {
            if is_file {
                disk.push( DiskNode { file_id: file_id } );
            } else {
                disk.push( DiskNode { file_id: -1 } );
            }
        }

        if is_file {
            file_id += 1;
        }

        is_file = !is_file;
    }

    //print_disk(&disk);

    // Defragment disk
    let mut empty_idx: usize = 0;
    let mut node_idx: usize = disk.len() - 1;
    let mut count = 0;
    let mut checksum: u64 = 0;
    let not_done = true;

    while not_done {
        count += 1;

        // Find first empty index
        for i in empty_idx..disk.len() {
            if disk[i].file_id == -1 {
                empty_idx = i;
                break;
            } else {
                checksum += i as u64 * disk[i].file_id as u64;
            }
        }

        // Find rigfht most file node
        for i in (0..node_idx+1).rev() {
            if disk[i].file_id != -1 {
                node_idx = i;
                break;
            }
        }

        if empty_idx > node_idx {
            break;
        }

        disk[empty_idx].file_id = disk[node_idx].file_id;
        disk[node_idx].file_id = -1;
    }

    //print_disk(&disk);
    println!("buffer length {}", input.len());
    println!("Max file id: {}", file_id);
    println!("iterations: {}", count);
    println!("checksum: {}", checksum);
}

#[allow(dead_code)]
fn print_disk(disk: &Vec<DiskNode>) {
    for node in disk {
        if node.file_id == -1 {
            print!(".");
        } else {
            print!("{}", node.file_id);
        }
    }
    println!();
}
