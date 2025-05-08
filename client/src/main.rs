use ftdi_embedded_hal::eh1::i2c::I2c;
use ftdi_embedded_hal::{FtHal, ftdi};

const I2C_SLAVE_ADDRESS: u8 = 0x28;
const I2C_CMD_READ_LASER_TIME_US: u8 = 0x01;

fn main() {
	let device = ftdi::find_by_vid_pid(0x0403, 0x6014)
		.interface(ftdi::Interface::A)
		.open()
		.unwrap();

	let hal = FtHal::init_freq(device, 100_000).unwrap();
	let mut i2c = hal.i2c().unwrap();

	let mut buf = [0u8; 8];
	let result = I2c::write_read(
		&mut i2c,
		I2C_SLAVE_ADDRESS,
		&[I2C_CMD_READ_LASER_TIME_US],
		&mut buf,
	);

	if result.is_ok() {
		let value = u64::from_le_bytes(buf);
		println!("{result:?} {buf:?} {value}");
	} else {
		println!("{result:?}");
	}
}
