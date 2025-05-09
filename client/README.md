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
0x28 0x01 Ok(()) [13, 141, 119, 9, 0, 0, 0, 0] [CRC 0x2f/0x2f Ok] 158829837µs
0x28 0x02 Ok(()) [126, 108, 2, 0, 0, 0, 0] [CRC 0x26/0x26 Ok] 158846ms
0x28 0x03 Ok(()) [52, 6, 0, 0, 0, 0] [CRC 0x1f/0x1f Ok] 1588ds
```
