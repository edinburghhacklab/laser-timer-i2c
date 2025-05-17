use std::fmt::Debug;

use ftdi_embedded_hal::eh1::i2c::I2c;
use ftdi_embedded_hal::{FtHal, ftdi};

const TARGET_ADDR: u8 = 0x28;
const CMD_READ_TOTL_US: u8 = 0x01;
const CMD_READ_TOTL_MS: u8 = 0x02;
const CMD_READ_TOTL_DS: u8 = 0x03;
const CMD_READ_CURR_NONE: u8 = 0x00;
const CMD_READ_CURR_U8_S: u8 = 0x04;
const CMD_READ_CURR_U16_S: u8 = 0x08;
const CMD_READ_CURR_U16_DS: u8 = 0x0C;

enum Status {
	Off,
	On { runtime: u64, units: String },
}

impl From<(u8, &str)> for Status {
	fn from(value: (u8, &str)) -> Self {
		match value.0 {
			0..u8::MAX => Self::On {
				runtime: value.0.into(),
				units: value.1.to_string(),
			},
			u8::MAX => Self::Off {},
		}
	}
}

impl From<(u16, &str)> for Status {
	fn from(value: (u16, &str)) -> Self {
		match value.0 {
			0..u16::MAX => Self::On {
				runtime: value.0.into(),
				units: value.1.to_string(),
			},
			u16::MAX => Self::Off {},
		}
	}
}

impl Debug for Status {
	fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
		match self {
			Self::On { runtime, units } => write!(f, "On for {runtime}{units}+"),
			Self::Off => write!(f, "Off"),
		}
	}
}

fn read<T: I2c>(
	i2c: &mut T,
	total_time_units: &str,
	current_time_len: usize,
	current_time_units: &str,
) {
	let addr = TARGET_ADDR;
	let (cmd, total_time_len) = match total_time_units {
		"µs" => (CMD_READ_TOTL_US, 8),
		"ms" => (CMD_READ_TOTL_MS, 7),
		"ds" => (CMD_READ_TOTL_DS, 6),
		_ => panic!("Unknown total time units: {total_time_units}"),
	};
	let cmd = cmd
		| match (current_time_len, current_time_units) {
			(0, "") => CMD_READ_CURR_NONE,
			(1, "s") => CMD_READ_CURR_U8_S,
			(2, "s") => CMD_READ_CURR_U16_S,
			(2, "ds") => CMD_READ_CURR_U16_DS,
			_ => panic!("Unknown current time len/units: {current_time_len}/{current_time_units}"),
		};
	let mut buf = [0u8; 11];
	let data_len = total_time_len + current_time_len;
	let read_len = data_len + 1;
	let result = I2c::write_read(i2c, addr, &[cmd], &mut buf[0..read_len]);

	if result.is_ok() {
		let data1 = buf[0..total_time_len].to_vec();
		let data2 = buf[total_time_len..total_time_len + current_time_len].to_vec();

		let crc_actual = buf[data_len];
		let crc8 = crc::Crc::<u8>::new(&crc::CRC_8_I_432_1);
		let mut crc_digest = crc8.digest();
		crc_digest.update(&buf[0..data_len]);
		let crc_expected = crc_digest.finalize();
		let crc_status = if crc_actual == crc_expected {
			"Ok"
		} else {
			"Err"
		};

		let mut total_time_value = [0u8; 8];
		total_time_value[0..total_time_len].copy_from_slice(data1.as_slice());
		let total_time_value = u64::from_le_bytes(total_time_value);

		let mut current_time_value = [0u8; 2];
		current_time_value[0..current_time_len].copy_from_slice(data2.as_slice());

		let status: Option<Status> = match current_time_len {
			0 => None,
			1 => Some((current_time_value[0], current_time_units).into()),
			2 => Some((u16::from_le_bytes(current_time_value), current_time_units).into()),
			_ => panic!("Unknown current time len: {current_time_len}"),
		};

		match status {
			None => println!(
				"0x{addr:02x} 0x{cmd:02x} {result:?} {data1:?} \
				[CRC 0x{crc_actual:02x}/0x{crc_expected:02x} {crc_status}] \
				Total: {total_time_value}{total_time_units}"
			),
			Some(status) => println!(
				"0x{addr:02x} 0x{cmd:02x} {result:?} {data1:?} {data2:?} \
				[CRC 0x{crc_actual:02x}/0x{crc_expected:02x} {crc_status}] \
				Total: {total_time_value}{total_time_units}, Current: {status:?}",
			),
		}
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
	let mut first = true;

	for total_time_units in ["µs", "ms", "ds"] {
		if first {
			first = false;
		} else {
			println!();
		}

		read(&mut i2c, total_time_units, 0, "");
		read(&mut i2c, total_time_units, 1, "s");
		read(&mut i2c, total_time_units, 2, "s");
		read(&mut i2c, total_time_units, 2, "ds");
	}
}
