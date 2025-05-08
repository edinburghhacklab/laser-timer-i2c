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
    Finished `release` profile [optimized] target(s) in 0.12s
     Running `target/release/client`
Ok(()) [37, 54, 177, 1, 0, 0, 0, 0] 28390949
```
