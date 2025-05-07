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
