# laser-timer-i2c

## Overview
Monitor laser cutter runtime using a Raspberry Pi Pico.

## Dependencies
[PlatformIO](https://platformio.org/)

## Build
`platformio run`

## Install
`platformio run -t upload`

## Hardware interface
The state is monitored using GPIO 22 and the total runtime in microseconds is
calculated while this pin is active.

The I2C device uses the default SDA/SCL pins GP4/GP5 and has address 0x28,
expecting to run at 100kHz.

## I2C interface
The laser runtime is stored in a buffer which must be refreshed before reading.

Write a command byte `0x01` to refresh the cached value (and reset the read
offset to the start of the buffer) and then read 6-8 bytes to get the value in
little-endian byte order.

Different precision levels are available:
* `0x01`: microseconds (8 bytes)
* `0x02`: milliseconds (7 bytes)
* `0x03`: deciseconds (6 bytes)
