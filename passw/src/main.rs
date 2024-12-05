use std::{time::SystemTime, u128};

fn gen_passw(instant: SystemTime) -> String {
    let strchars: Vec<char> =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890@._-^*¿?"
            .chars()
            .collect();
    let charset_len = strchars.len();

    let mut password = String::new();

    let mut seed = instant.elapsed().unwrap().as_nanos();

    let max_iterations = 1000; // Limit to 1000 iterations to avoid infinite loop
    let mut iterations = 0;

    while seed < u128::MAX && iterations < max_iterations {
        seed = seed
            .wrapping_mul(seed % 100)
            .wrapping_mul(seed % 1000)
            .wrapping_mul((seed.wrapping_mul(5567)) % 1000);

        if seed == 0 {
            seed = SystemTime::now().elapsed().unwrap().as_nanos();
        }

        iterations += 1;
    }

    for _ in 0..30 {
        let index = (seed % charset_len as u128) as usize;
        password.push(strchars[index]);
        seed /= charset_len as u128;
        if seed == 0 {
            seed = SystemTime::now().elapsed().unwrap().as_nanos() as u128;
        }
    }

    password
}

fn main() {
    let instant = SystemTime::now();
    println!("{}", gen_passw(instant));
}
