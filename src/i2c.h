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

#pragma once

#include <Arduino.h>
#include <hardware/i2c.h>

class LaserTime;

class I2C: public Printable {
public:
    I2C(LaserTime &laser, uint8_t address, unsigned int speed,
        unsigned int sda, unsigned int scl);

    void start();
    size_t printTo(Print &p) const override;

private:
    static constexpr uint8_t I2C_CMD_READ_TOTAL_TIME_US = 0x01;
    static constexpr uint8_t I2C_CMD_READ_TOTAL_TIME_MS = 0x02;
    static constexpr uint8_t I2C_CMD_READ_TOTAL_TIME_DS = 0x03;

    static constexpr uint8_t I2C_CMD_READ_CURRENT_TIME_MASK = 0x0C;
    static constexpr uint8_t I2C_CMD_READ_CURRENT_TIME_NONE = 0x00;
    static constexpr uint8_t I2C_CMD_READ_CURRENT_TIME_U8_S = 0x04;
    static constexpr uint8_t I2C_CMD_READ_CURRENT_TIME_U16_S = 0x08;
    static constexpr uint8_t I2C_CMD_READ_CURRENT_TIME_U16_DS = 0x0C;

    friend void __not_in_flash_func(c_i2c_target_irq_handler)();
    void __not_in_flash_func(i2c_target_irq_handler)();

    void __not_in_flash_func(rx)(uint8_t data);
    uint8_t __not_in_flash_func(tx)();
    void __not_in_flash_func(reset)();

    const LaserTime &laser_;
    const uint8_t address_;
    const unsigned int speed_;
    const unsigned int sda_;
    const unsigned int scl_;
    i2c_hw_t * const hw_;

    uint8_t value_[10];
    uint8_t value_idx_ = 0;
    uint8_t value_len_ = 0;
    uint8_t crc_ = 0;

    unsigned long abort_count_ = 0;
    unsigned long start_count_ = 0;
    unsigned long stop_count_ = 0;
    unsigned long read_count_ = 0;
    unsigned long unknown_count_ = 0;
    unsigned long cmd_count_ = 0;
    unsigned long write_count_ = 0;
};
