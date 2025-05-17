/*
 * Copyright 2025  Simon Arlott
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "i2c.h"

#include <Arduino.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>

#include <algorithm>

#include <cppcrc.h>

#include "laser.h"

static I2C *instance = nullptr;

I2C::I2C(LaserTime &laser, uint8_t address, unsigned int speed,
		unsigned int sda, unsigned int scl) : laser_(laser), address_(address),
		speed_(speed), sda_(sda), scl_(scl), hw_(i2c_get_hw(i2c0)) {

}

void __not_in_flash_func(c_i2c_target_irq_handler)() {
	instance->i2c_target_irq_handler();
}

void I2C::start() {
	_gpio_init(sda_);
	gpio_set_function(sda_, GPIO_FUNC_I2C);
	gpio_pull_up(sda_);

	_gpio_init(scl_);
	gpio_set_function(scl_, GPIO_FUNC_I2C);
	gpio_pull_up(scl_);

	_i2c_init(i2c0, speed_);
	i2c_set_slave_mode(i2c0, true, address_);

	hw_->rx_tl = 0;
	hw_->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS
		| I2C_IC_INTR_MASK_M_RD_REQ_BITS
		| I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS
		| I2C_IC_INTR_MASK_M_STOP_DET_BITS
		| I2C_IC_INTR_MASK_M_START_DET_BITS;

	instance = this;
	irq_set_exclusive_handler(I2C0_IRQ, &c_i2c_target_irq_handler);
	irq_set_enabled(I2C0_IRQ, true);
}

inline void __not_in_flash_func(I2C::rx)(uint8_t data) {
	LaserStatus status = laser_.status();
	uint64_t total_time;
	size_t total_time_len;
	uint16_t current_time;
	size_t current_time_len;

	switch (data & ~I2C_CMD_READ_CURRENT_TIME_MASK) {
	case I2C_CMD_READ_TOTAL_TIME_US: /* microseconds */
		total_time = status.total_time_us;
		total_time_len = 8;
		break;

	case I2C_CMD_READ_TOTAL_TIME_MS: /* milliseconds */
		total_time = status.total_time_us / 1000ULL;
		total_time_len = 7;
		break;

	case I2C_CMD_READ_TOTAL_TIME_DS: /* deciseconds */
		total_time = status.total_time_us / 100000ULL;
		total_time_len = 6;
		break;

	default:
		unknown_count_++;
		return;
	}

	value_len_ = total_time_len;

	switch (data & I2C_CMD_READ_CURRENT_TIME_MASK) {
	case I2C_CMD_READ_CURRENT_TIME_NONE:
		current_time_len = 0;
		break;

	case I2C_CMD_READ_CURRENT_TIME_U8_S: /* seconds (0-254) */
		current_time = status.on ? std::min((uint64_t)(UINT8_MAX - 1),
			status.current_time_us / 1000000ULL) : UINT8_MAX;
		current_time_len = 1;
		break;

	case I2C_CMD_READ_CURRENT_TIME_U16_S: /* seconds (0-65535) */
		current_time = status.on ? std::min((uint64_t)(UINT16_MAX - 1),
			status.current_time_us / 1000000ULL) : UINT16_MAX;
		current_time_len = 2;
		break;

	case I2C_CMD_READ_CURRENT_TIME_U16_DS: /* deciseconds (0-65535) */
		current_time = status.on ? std::min((uint64_t)(UINT16_MAX - 1),
			status.current_time_us / 100000ULL) : UINT16_MAX;
		current_time_len = 2;
		break;

	default:
		/* unreachable */
		unknown_count_++;
		return;
	}

	cmd_count_++;

	static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
	static_assert(sizeof(value_) >= sizeof(total_time));
	memcpy(value_, &total_time, total_time_len);

	while (current_time_len > 0) {
		value_[value_len_++] = current_time;
		current_time >>= 8;
		current_time_len--;
	}

	value_idx_ = 0;
	crc_ = CRC8::ITU::null_crc;
}

inline uint8_t __not_in_flash_func(I2C::tx)() {
	uint8_t data = 0;

	if (value_idx_ < value_len_) {
		data = value_[value_idx_++];
		crc_ = CRC8::ITU::calc(&data, 1, crc_);
	} else {
		data = crc_;
		crc_ = 0;
	}

	return data;
}

inline void __not_in_flash_func(I2C::reset)() {
	value_idx_ = 0;
	value_len_ = 0;
	crc_ = 0;
}

void __not_in_flash_func(I2C::i2c_target_irq_handler()) {
	uint32_t intr_stat = hw_->intr_stat;

	if (intr_stat & I2C_IC_INTR_STAT_R_TX_ABRT_BITS) {
		abort_count_++;
		reset();
		hw_->clr_tx_abrt;
	}

	if (intr_stat & I2C_IC_INTR_STAT_R_START_DET_BITS) {
		start_count_++;
		hw_->clr_start_det;
	}

	if (intr_stat & I2C_IC_INTR_STAT_R_STOP_DET_BITS) {
		stop_count_++;
		hw_->clr_stop_det;
	}

	if (intr_stat & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
		read_count_++;
		rx(hw_->data_cmd);
	}

	if (intr_stat & I2C_IC_INTR_STAT_R_RD_REQ_BITS) {
		write_count_++;
		hw_->clr_rd_req;
		hw_->data_cmd = tx();
	}
}

size_t I2C::printTo(Print &p) const {
	size_t len = 0;

	len += p.print("{abort: ");
	len += p.print(abort_count_);
	len += p.print(", start: ");
	len += p.print(start_count_);
	len += p.print(", stop: ");
	len += p.print(stop_count_);
	len += p.print(", read: ");
	len += p.print(read_count_);
	len += p.print(", cmd: ");
	len += p.print(cmd_count_);
	len += p.print(", unknown: ");
	len += p.print(unknown_count_);
	len += p.print(", write: ");
	len += p.print(write_count_);
	len += p.print("}");

	return len;
}
