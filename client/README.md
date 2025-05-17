# I2C test client

## Overview
Query the laser runtime in microseconds using I2C.

## Dependencies
Rust 1.85.0+

## Hardware interface
FTDI FT232H, e.g. C232HM-DDHSL-0 or C232HD-DDHSP-0

## Build
`cargo build -r`

## Usage
`cargo run -qr`

### Laser off, total 0s
```
0x28 0x01 Ok(()) [0, 0, 0, 0, 0, 0, 0, 0] [CRC 0x55/0x55 Ok] Total: 0µs
0x28 0x05 Ok(()) [0, 0, 0, 0, 0, 0, 0, 0] [255] [CRC 0xa6/0xa6 Ok] Total: 0µs, Current: Off
0x28 0x09 Ok(()) [0, 0, 0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0µs, Current: Off
0x28 0x0d Ok(()) [0, 0, 0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0µs, Current: Off

0x28 0x02 Ok(()) [0, 0, 0, 0, 0, 0, 0] [CRC 0x55/0x55 Ok] Total: 0ms
0x28 0x06 Ok(()) [0, 0, 0, 0, 0, 0, 0] [255] [CRC 0xa6/0xa6 Ok] Total: 0ms, Current: Off
0x28 0x0a Ok(()) [0, 0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0ms, Current: Off
0x28 0x0e Ok(()) [0, 0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0ms, Current: Off

0x28 0x03 Ok(()) [0, 0, 0, 0, 0, 0] [CRC 0x55/0x55 Ok] Total: 0ds
0x28 0x07 Ok(()) [0, 0, 0, 0, 0, 0] [255] [CRC 0xa6/0xa6 Ok] Total: 0ds, Current: Off
0x28 0x0b Ok(()) [0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0ds, Current: Off
0x28 0x0f Ok(()) [0, 0, 0, 0, 0, 0] [255, 255] [CRC 0x71/0x71 Ok] Total: 0ds, Current: Off
```

### Laser off, total 14.5s
```
0x28 0x01 Ok(()) [179, 142, 222, 0, 0, 0, 0, 0] [CRC 0x97/0x97 Ok] Total: 14585523µs
0x28 0x05 Ok(()) [179, 142, 222, 0, 0, 0, 0, 0] [255] [CRC 0xe6/0xe6 Ok] Total: 14585523µs, Current: Off
0x28 0x09 Ok(()) [179, 142, 222, 0, 0, 0, 0, 0] [255, 255] [CRC 0xb6/0xb6 Ok] Total: 14585523µs, Current: Off
0x28 0x0d Ok(()) [179, 142, 222, 0, 0, 0, 0, 0] [255, 255] [CRC 0xb6/0xb6 Ok] Total: 14585523µs, Current: Off

0x28 0x02 Ok(()) [249, 56, 0, 0, 0, 0, 0] [CRC 0x37/0x37 Ok] Total: 14585ms
0x28 0x06 Ok(()) [249, 56, 0, 0, 0, 0, 0] [255] [CRC 0x8f/0x8f Ok] Total: 14585ms, Current: Off
0x28 0x0a Ok(()) [249, 56, 0, 0, 0, 0, 0] [255, 255] [CRC 0xae/0xae Ok] Total: 14585ms, Current: Off
0x28 0x0e Ok(()) [249, 56, 0, 0, 0, 0, 0] [255, 255] [CRC 0xae/0xae Ok] Total: 14585ms, Current: Off

0x28 0x03 Ok(()) [145, 0, 0, 0, 0, 0] [CRC 0x0e/0x0e Ok] Total: 145ds
0x28 0x07 Ok(()) [145, 0, 0, 0, 0, 0] [255] [CRC 0x20/0x20 Ok] Total: 145ds, Current: Off
0x28 0x0b Ok(()) [145, 0, 0, 0, 0, 0] [255, 255] [CRC 0xea/0xea Ok] Total: 145ds, Current: Off
0x28 0x0f Ok(()) [145, 0, 0, 0, 0, 0] [255, 255] [CRC 0xea/0xea Ok] Total: 145ds, Current: Off
```

### Laser on within the last second, total 63.9s
```
0x28 0x01 Ok(()) [12, 44, 204, 3, 0, 0, 0, 0] [CRC 0x68/0x68 Ok] Total: 63712268µs
0x28 0x05 Ok(()) [23, 111, 204, 3, 0, 0, 0, 0] [0] [CRC 0xc0/0xc0 Ok] Total: 63729431µs, Current: On for 0s+
0x28 0x09 Ok(()) [124, 182, 204, 3, 0, 0, 0, 0] [0, 0] [CRC 0x30/0x30 Ok] Total: 63747708µs, Current: On for 0s+
0x28 0x0d Ok(()) [48, 252, 204, 3, 0, 0, 0, 0] [0, 0] [CRC 0xb1/0xb1 Ok] Total: 63765552µs, Current: On for 0ds+

0x28 0x02 Ok(()) [40, 249, 0, 0, 0, 0, 0] [CRC 0x47/0x47 Ok] Total: 63784ms
0x28 0x06 Ok(()) [58, 249, 0, 0, 0, 0, 0] [0] [CRC 0x3a/0x3a Ok] Total: 63802ms, Current: On for 0s+
0x28 0x0a Ok(()) [76, 249, 0, 0, 0, 0, 0] [0, 0] [CRC 0xdc/0xdc Ok] Total: 63820ms, Current: On for 0s+
0x28 0x0e Ok(()) [95, 249, 0, 0, 0, 0, 0] [1, 0] [CRC 0xc7/0xc7 Ok] Total: 63839ms, Current: On for 1ds+

0x28 0x03 Ok(()) [126, 2, 0, 0, 0, 0] [CRC 0xfb/0xfb Ok] Total: 638ds
0x28 0x07 Ok(()) [126, 2, 0, 0, 0, 0] [0] [CRC 0x16/0x16 Ok] Total: 638ds, Current: On for 0s+
0x28 0x0b Ok(()) [126, 2, 0, 0, 0, 0] [0, 0] [CRC 0x9b/0x9b Ok] Total: 638ds, Current: On for 0s+
0x28 0x0f Ok(()) [127, 2, 0, 0, 0, 0] [2, 0] [CRC 0xa2/0xa2 Ok] Total: 639ds, Current: On for 2ds+
```

### Laser on for over 42s, total 106.3s
```
0x28 0x01 Ok(()) [16, 54, 83, 6, 0, 0, 0, 0] [CRC 0x04/0x04 Ok] Total: 106116624µs
0x28 0x05 Ok(()) [58, 122, 83, 6, 0, 0, 0, 0] [42] [CRC 0x05/0x05 Ok] Total: 106134074µs, Current: On for 42s+
0x28 0x09 Ok(()) [21, 195, 83, 6, 0, 0, 0, 0] [42, 0] [CRC 0x93/0x93 Ok] Total: 106152725µs, Current: On for 42s+
0x28 0x0d Ok(()) [212, 9, 84, 6, 0, 0, 0, 0] [168, 1] [CRC 0xce/0xce Ok] Total: 106170836µs, Current: On for 424ds+

0x28 0x02 Ok(()) [205, 158, 1, 0, 0, 0, 0] [CRC 0x73/0x73 Ok] Total: 106189ms
0x28 0x06 Ok(()) [224, 158, 1, 0, 0, 0, 0] [42] [CRC 0xd8/0xd8 Ok] Total: 106208ms, Current: On for 42s+
0x28 0x0a Ok(()) [242, 158, 1, 0, 0, 0, 0] [42, 0] [CRC 0x88/0x88 Ok] Total: 106226ms, Current: On for 42s+
0x28 0x0e Ok(()) [5, 159, 1, 0, 0, 0, 0] [169, 1] [CRC 0xdb/0xdb Ok] Total: 106245ms, Current: On for 425ds+

0x28 0x03 Ok(()) [38, 4, 0, 0, 0, 0] [CRC 0x17/0x17 Ok] Total: 1062ds
0x28 0x07 Ok(()) [38, 4, 0, 0, 0, 0] [42] [CRC 0x4a/0x4a Ok] Total: 1062ds, Current: On for 42s+
0x28 0x0b Ok(()) [39, 4, 0, 0, 0, 0] [42, 0] [CRC 0x1b/0x1b Ok] Total: 1063ds, Current: On for 42s+
0x28 0x0f Ok(()) [39, 4, 0, 0, 0, 0] [170, 1] [CRC 0xaa/0xaa Ok] Total: 1063ds, Current: On for 426ds+
```

### Laser on for over 300s, total 364s
```
0x28 0x01 Ok(()) [233, 141, 176, 21, 0, 0, 0, 0] [CRC 0x8f/0x8f Ok] Total: 363892201µs
0x28 0x05 Ok(()) [220, 211, 176, 21, 0, 0, 0, 0] [254] [CRC 0xa2/0xa2 Ok] Total: 363910108µs, Current: On for 254s+
0x28 0x09 Ok(()) [68, 27, 177, 21, 0, 0, 0, 0] [44, 1] [CRC 0xa2/0xa2 Ok] Total: 363928388µs, Current: On for 300s+
0x28 0x0d Ok(()) [54, 96, 177, 21, 0, 0, 0, 0] [186, 11] [CRC 0xf4/0xf4 Ok] Total: 363946038µs, Current: On for 3002ds+

0x28 0x02 Ok(()) [188, 141, 5, 0, 0, 0, 0] [CRC 0xf1/0xf1 Ok] Total: 363964ms
0x28 0x06 Ok(()) [206, 141, 5, 0, 0, 0, 0] [254] [CRC 0x77/0x77 Ok] Total: 363982ms, Current: On for 254s+
0x28 0x0a Ok(()) [225, 141, 5, 0, 0, 0, 0] [44, 1] [CRC 0x88/0x88 Ok] Total: 364001ms, Current: On for 300s+
0x28 0x0e Ok(()) [243, 141, 5, 0, 0, 0, 0] [187, 11] [CRC 0x43/0x43 Ok] Total: 364019ms, Current: On for 3003ds+

0x28 0x03 Ok(()) [56, 14, 0, 0, 0, 0] [CRC 0xed/0xed Ok] Total: 3640ds
0x28 0x07 Ok(()) [56, 14, 0, 0, 0, 0] [254] [CRC 0x80/0x80 Ok] Total: 3640ds, Current: On for 254s+
0x28 0x0b Ok(()) [56, 14, 0, 0, 0, 0] [44, 1] [CRC 0xe7/0xe7 Ok] Total: 3640ds, Current: On for 300s+
0x28 0x0f Ok(()) [56, 14, 0, 0, 0, 0] [188, 11] [CRC 0x30/0x30 Ok] Total: 3640ds, Current: On for 3004ds+
```

### Laser off, total 410.3s
```
0x28 0x01 Ok(()) [109, 252, 116, 24, 0, 0, 0, 0] [CRC 0x5e/0x5e Ok] Total: 410319981µs
0x28 0x05 Ok(()) [109, 252, 116, 24, 0, 0, 0, 0] [255] [CRC 0x97/0x97 Ok] Total: 410319981µs, Current: Off
0x28 0x09 Ok(()) [109, 252, 116, 24, 0, 0, 0, 0] [255, 255] [CRC 0xe6/0xe6 Ok] Total: 410319981µs, Current: Off
0x28 0x0d Ok(()) [109, 252, 116, 24, 0, 0, 0, 0] [255, 255] [CRC 0xe6/0xe6 Ok] Total: 410319981µs, Current: Off

0x28 0x02 Ok(()) [207, 66, 6, 0, 0, 0, 0] [CRC 0x0c/0x0c Ok] Total: 410319ms
0x28 0x06 Ok(()) [207, 66, 6, 0, 0, 0, 0] [255] [CRC 0x2e/0x2e Ok] Total: 410319ms, Current: Off
0x28 0x0a Ok(()) [207, 66, 6, 0, 0, 0, 0] [255, 255] [CRC 0xc0/0xc0 Ok] Total: 410319ms, Current: Off
0x28 0x0e Ok(()) [207, 66, 6, 0, 0, 0, 0] [255, 255] [CRC 0xc0/0xc0 Ok] Total: 410319ms, Current: Off

0x28 0x03 Ok(()) [7, 16, 0, 0, 0, 0] [CRC 0xb8/0xb8 Ok] Total: 4103ds
0x28 0x07 Ok(()) [7, 16, 0, 0, 0, 0] [255] [CRC 0x2b/0x2b Ok] Total: 4103ds, Current: Off
0x28 0x0b Ok(()) [7, 16, 0, 0, 0, 0] [255, 255] [CRC 0xdb/0xdb Ok] Total: 4103ds, Current: Off
0x28 0x0f Ok(()) [7, 16, 0, 0, 0, 0] [255, 255] [CRC 0xdb/0xdb Ok] Total: 4103ds, Current: Off
```
