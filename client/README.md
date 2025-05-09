# I2C test client

## Overview
Query the laser runtime in microseconds using I2C.

## Dependencies
Rust 1.85.0+

## Hardware interface
FTDI FT232H, e.g. C232HM-DDHSL-0

## Build
`cargo build -r`

## Usage
```
$ cargo run -r
    Finished `release` profile [optimized] target(s) in 0.02s
     Running `target/release/client`
0x28 0x01 Ok(()) [16, 252, 23, 16, 0, 0, 0, 0] 270007312µs
0x28 0x02 Ok(()) [201, 30, 4, 0, 0, 0, 0] 270025ms
0x28 0x03 Ok(()) [140, 10, 0, 0, 0, 0] 2700ds
```
