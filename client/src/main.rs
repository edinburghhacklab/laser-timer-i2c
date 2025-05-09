use ftdi_embedded_hal::eh1::i2c::I2c;
use ftdi_embedded_hal::{FtHal, ftdi};

const I2C_SLAVE_ADDRESS: u8 = 0x28;
const I2C_CMD_READ_LASER_TIME_US: u8 = 0x01;
const I2C_CMD_READ_LASER_TIME_MS: u8 = 0x02;
const I2C_CMD_READ_LASER_TIME_DS: u8 = 0x03;

fn read<T: I2c>(i2c: &mut T, addr: u8, cmd: u8, len: usize, units: &str) {
	let mut buf = [0u8; 8];
	let result = I2c::write_read(i2c, addr, &[cmd], &mut buf[0..len]);

	if result.is_ok() {
		let vec = buf[0..len].to_vec();
		let value = u64::from_le_bytes(buf);
		println!("0x{addr:02x} 0x{cmd:02x} {result:?} {vec:?} {value}{units}");
	} else {
		println!("0x{addr:02x} 0x{cmd:02x} {result:?}");
	}
}

fn main() {
	let device = ftdi::find_by_vid_pid(0x0403, 0x6014)
		.interface(ftdi::Interface::A)
		.open()
		.unwrap();

	let hal = FtHal::init_freq(device, 100_000).unwrap();
	let mut i2c = hal.i2c().unwrap();

	read(
		&mut i2c,
		I2C_SLAVE_ADDRESS,
		I2C_CMD_READ_LASER_TIME_US,
		8,
		"µs",
	);
	read(
		&mut i2c,
		I2C_SLAVE_ADDRESS,
		I2C_CMD_READ_LASER_TIME_MS,
		7,
		"ms",
	);
	read(
		&mut i2c,
		I2C_SLAVE_ADDRESS,
		I2C_CMD_READ_LASER_TIME_DS,
		6,
		"ds",
	);
}
