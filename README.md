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

Write a command byte `0x0D` to refresh the cached value (and reset the read
offset to the start of the buffer) and then read 11 bytes to get the total
runtime followed by the current runtime and then a CRC-8 (ITU I.432.1) on the
preceding data. Values are in little-endian byte order.

Different precision levels are available:
* `0x01`: total microseconds (8 bytes) + CRC (1 byte)
* `0x05`: total microseconds (8 bytes) + current seconds (1 byte) + CRC (1 byte)
* `0x09`: total microseconds (8 bytes) + current seconds (2 bytes) + CRC (1 byte)
* `0x0D`: total microseconds (8 bytes) + current deciseconds (2 bytes) + CRC (1 byte)

* `0x02`: total milliseconds (7 bytes) + CRC (1 byte)
* `0x06`: total milliseconds (7 bytes) + current seconds (1 byte) + CRC (1 byte)
* `0x0A`: total milliseconds (7 bytes) + current seconds (2 bytes) + CRC (1 byte)
* `0x0E`: total milliseconds (7 bytes) + current deciseconds (2 bytes) + CRC (1 byte)

* `0x03`: total deciseconds (6 bytes) + CRC (1 byte)
* `0x07`: total deciseconds (6 bytes) + current seconds (1 byte) + CRC (1 byte)
* `0x0B`: total deciseconds (6 bytes) + current seconds (2 bytes) + CRC (1 byte)
* `0x0F`: total deciseconds (6 bytes) + current deciseconds (2 bytes) + CRC (1 byte)

The current runtime will be the maximum value (255 or 65535) when the laser is
off, i.e. a 1 byte value of 0 indicates that the laser is on and has been on for
less than 1 second; a 1 byte value of 254 indicates that the laser is on and has
been on for at least 254 seconds.
