use chrono::{DateTime, Local};
use std::{
    env, fmt, fs,
    hash::{DefaultHasher, Hash, Hasher},
    io::{self, Write},
    path::PathBuf,
    str::FromStr,
    time::SystemTime,
};

const FILE_PATH: &str = "/home/lvx/.config/doers/todo.txt";

fn calculate_hash<T: Hash>(t: &T) -> u64 {
    let mut hasher = DefaultHasher::new();
    t.hash(&mut hasher);
    hasher.finish()
}

struct Item {
    creationdate: DateTime<Local>,
    contents: String,
    header: String,
    hash: u64,
}

impl Item {
    pub fn new(header: String, contents: String) -> Item {
        Item {
            creationdate: DateTime::<Local>::from(SystemTime::now()),
            contents,
            hash: calculate_hash(&header),
            header,
        }
    }

    pub fn write(&self) {
        let mut file = fs::OpenOptions::new()
            .create(true)
            .append(true)
            .write(true)
            .open(PathBuf::from_str(FILE_PATH).unwrap())
            .unwrap();
        file.write_all(
            format!(
                "{}%{}%{}%{}\n",
                self.creationdate,
                self.hash,
                self.header,
                self.contents.trim()
            )
            .as_bytes(),
        )
        .unwrap();
    }
}

impl fmt::Display for Item {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let f_contents = self
            .contents
            .split_whitespace()
            .collect::<Vec<&str>>()
            .chunks(10)
            .map(|chunk| chunk.join(" "))
            .collect::<Vec<String>>()
            .join("\n\t");

        write!(
            f,
            "\x1b[32m[{}]\x1b[0m\n{}:\n\t{}\n{}\n",
            self.hash,
            self.header,
            f_contents,
            self.creationdate.to_rfc2822(),
        )
    }
}

fn read_file() -> Vec<Item> {
    let mut outv: Vec<Item> = Vec::new();
    let file =
        fs::read_to_string(PathBuf::from_str(FILE_PATH).unwrap_or_else(|_| "".into())).unwrap();
    for l in file.lines() {
        let parsed: Vec<&str> = l.split('%').collect();
        if parsed.len() == 4 {
            let creationdate =
                DateTime::<Local>::from_str(parsed[0]).unwrap_or_else(|_| Local::now());
            let hash = parsed[1].parse().unwrap_or_default();
            let header = parsed[2].to_string();
            let contents = parsed[3].to_string();
            outv.push(Item {
                creationdate,
                hash,
                header,
                contents,
            });
        }
    }
    outv
}

fn write_all_items(items: &[Item]) {
    let mut file = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(PathBuf::from_str(FILE_PATH).unwrap())
        .unwrap();

    for item in items {
        file.write_all(
            format!(
                "{}%{}%{}%{}\n",
                item.creationdate,
                item.hash,
                item.header,
                item.contents.trim()
            )
            .as_bytes(),
        )
        .unwrap();
    }
}

fn delete_item(hash: u64) {
    let mut items = read_file();
    if let Some(index) = items.iter().position(|i| i.hash == hash) {
        println!("Deleting item:\n{}", items[index]);
        items.remove(index);
        write_all_items(&items);
        println!("Item deleted successfully.");
    } else {
        println!("No item found with hash: {}", hash);
    }
}

fn add_item() {
    println!("Enter the task header: ");
    let mut header = String::new();
    io::stdin().read_line(&mut header).unwrap();

    println!("Enter the task contents: ");
    let mut contents = String::new();
    io::stdin().read_line(&mut contents).unwrap();

    let new_item = Item::new(header.trim().to_string(), contents.trim().to_string());
    new_item.write();
    println!("Task added successfully:\n{}", new_item);
}

struct Opts {
    modify: bool,
    modhash: u64,
    add: bool,
    delete: bool,
    delhash: u64,
}

fn parse_args(argv: Vec<String>) -> Opts {
    let mut opts = Opts {
        modify: false,
        modhash: 0,
        add: false,
        delete: false,
        delhash: 0,
    };

    for i in 0..argv.len() {
        let ar = argv[i].as_str();
        match ar {
            "-m" => {
                opts.modify = true;
                opts.modhash = argv[i + 1].parse().unwrap_or(0);
            }
            "-a" => opts.add = true,
            "-d" => {
                opts.delete = true;
                opts.delhash = argv[i + 1].parse().unwrap_or(0);
            }
            &_ => (),
        }
    }
    opts
}

fn main() {
    let argv: Vec<String> = env::args().collect();
    let opts = parse_args(argv);

    if opts.modify {
        modify_item(opts.modhash);
    } else if opts.add {
        add_item();
    } else if opts.delete {
        delete_item(opts.delhash);
    } else {
        let items = read_file();
        for item in items {
            print!("{}", item);
        }
    }
}

fn modify_item(hash: u64) {
    let mut items = read_file();
    if let Some(item) = items.iter_mut().find(|i| i.hash == hash) {
        println!("Found item:\n{}", item);
        println!("Enter new header (leave blank to keep current): ");
        let mut new_header = String::new();
        io::stdin().read_line(&mut new_header).unwrap();
        if !new_header.trim().is_empty() {
            item.header = new_header.trim().to_string();
            item.hash = calculate_hash(&item.header); // Update hash as header has changed
        }

        println!("Enter new contents (leave blank to keep current): ");
        let mut new_contents = String::new();
        io::stdin().read_line(&mut new_contents).unwrap();
        if !new_contents.trim().is_empty() {
            item.contents = new_contents.trim().to_string();
        }

        println!("Item updated:\n{}", item);
    } else {
        println!("No item found with hash: {}", hash);
    }

    write_all_items(&items);
}
