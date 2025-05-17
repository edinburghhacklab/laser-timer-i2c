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

#include <Arduino.h>
#include <hardware/i2c.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <pico/multicore.h>
#pragma GCC diagnostic pop

#include "i2c.h"
#include "laser.h"

static constexpr unsigned int LASER_GPIO = 22;
//static constexpr bool LASER_ACTIVE_LOW = true; /* direct connection to laser signal */
static constexpr bool LASER_ACTIVE_LOW = false; /* when using an optocoupler */
static constexpr uint8_t I2C_ADDRESS = 0x28;

static LaserTime laser{LASER_GPIO, LASER_ACTIVE_LOW};
static I2C i2c{
	laser, I2C_ADDRESS, 100000,
	PICO_DEFAULT_I2C_SDA_PIN /* 4 */,
	PICO_DEFAULT_I2C_SCL_PIN /* 5 */
};

void setup() {
	laser.start();
	multicore_launch_core1([] { i2c.start(); });
}

void loop() {
	Serial.print("{uptime: ");
	Serial.print(LaserTime::uptime_us());
	Serial.print(", laser: ");
	laser.printTo(Serial);
	Serial.print(", i2c: ");
	i2c.printTo(Serial);
	Serial.println("}");
	sleep_ms(1000);
}
