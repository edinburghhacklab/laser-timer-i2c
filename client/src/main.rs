use ftdi_embedded_hal::eh1::i2c::I2c;
use ftdi_embedded_hal::{FtHal, ftdi};

const SLAVE_ADDRESS: u8 = 0x28;
const CMD_READ_LASER_TIME_US: u8 = 0x01;
const CMD_READ_LASER_TIME_MS: u8 = 0x02;
const CMD_READ_LASER_TIME_DS: u8 = 0x03;

fn read<T: I2c>(i2c: &mut T, addr: u8, cmd: u8, len: usize, units: &str) {
	let mut buf = [0u8; 9];
	let result = I2c::write_read(i2c, addr, &[cmd], &mut buf[0..len + 1]);

	if result.is_ok() {
		let vec = buf[0..len].to_vec();

		let crc_actual = buf[len];
		let crc8 = crc::Crc::<u8>::new(&crc::CRC_8_I_432_1);
		let mut crc_digest = crc8.digest();
		crc_digest.update(&buf[0..len]);
		let crc_expected = crc_digest.finalize();
		buf[len] = 0;

		let mut value = [0u8; 8];
		value.copy_from_slice(&buf[0..8]);
		let value = u64::from_le_bytes(value);

		println!(
			"0x{addr:02x} 0x{cmd:02x} {result:?} {vec:?} [CRC 0x{crc_actual:02x}/0x{crc_expected:02x} {}] {value}{units}",
			if crc_actual == crc_expected {
				"Ok"
			} else {
				"Err"
			}
		);
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

	read(&mut i2c, SLAVE_ADDRESS, CMD_READ_LASER_TIME_US, 8, "µs");
	read(&mut i2c, SLAVE_ADDRESS, CMD_READ_LASER_TIME_MS, 7, "ms");
	read(&mut i2c, SLAVE_ADDRESS, CMD_READ_LASER_TIME_DS, 6, "ds");
}
