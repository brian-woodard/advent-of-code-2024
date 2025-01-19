use std::fs::File;
use std::io::BufReader;
use std::io::BufRead;

#[derive(Debug, Copy, Clone)]
struct DiskNode {
    file_id: i32,
    size: i32,
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

    let mut file_id = 0;
    let mut max_file_id = 0;
    let mut is_file = true;
    for i in 0..input.len() {
        let size: i32 = input[i] as i32 - b'0' as i32;

        if size <= 0 || size > 9 {
            is_file = !is_file;
            continue;
        }

        if is_file {
            max_file_id = file_id;
            disk.push( DiskNode { file_id: file_id, size: size } );
            file_id += 1;
        } else {
            disk.push( DiskNode { file_id: -1, size: size } );
        }

        is_file = !is_file;
    }

    //print_disk(&disk);

    file_id = max_file_id;
    loop {

        let mut empty_idx = 0;
        let mut node_idx = disk.len() - 1;

        // Find first empty node
        for i in 0..disk.len() {
            if disk[i].file_id == -1 {
                empty_idx = i;
                break;
            }
        }

        // Find start index for node
        for i in (0..node_idx+1).rev() {
            if disk[i].file_id == file_id {
                node_idx = i;
                break;
            }
        }

        if node_idx < empty_idx || file_id < 0 {
            break;
        }

        // Now search from left for an empty block that can fit the node
        for i in empty_idx..node_idx {
            if disk[i].file_id == -1 && disk[i].size >= disk[node_idx].size {
                let split_size = disk[i].size - disk[node_idx].size;

                // Swap nodes
                let tmp = disk[i];
                disk[i] = disk[node_idx];
                disk[node_idx] = tmp;

                // If empty space was larger, add an empty spot after, and fix size
                if split_size != 0 {
                    disk[node_idx].size -= split_size;
                    disk.insert(i + 1, DiskNode { file_id: -1, size: split_size } );
                }

                //print_disk(&disk);

                break;
            }
        }

        file_id -= 1;
    }

    //print_disk(&disk);

    let mut checksum: u64 = 0;
    let mut idx: u64 = 0;
    for node in disk {
        if node.file_id == -1 {
            idx += node.size as u64;
        } else {
            for _i in 0..node.size {
                checksum += node.file_id as u64 * idx;
                idx += 1;
            }
        }
    }

    println!("buffer length {}", input.len());
    println!("max file id: {}", max_file_id);
    println!("checksum: {}", checksum);
}

#[allow(dead_code)]
fn print_disk(disk: &Vec<DiskNode>) {
    for node in disk {
        if node.file_id == -1 {
            for _i in 0..node.size {
                print!(".");
            }
        } else {
            for _i in 0..node.size {
                print!("{}", node.file_id);
            }
        }
    }
    println!("\nSize: {}", disk.len());
}
