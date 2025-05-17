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
#include <hardware/sync.h>

struct LaserStatus {
	uint64_t total_time_us;
	uint64_t current_time_us;
	bool on;
};

class LaserTime: public Printable {
public:
	LaserTime(unsigned int gpio, bool active_low);

	static uint64_t __not_in_flash_func(uptime_us)();

	void start();
	LaserStatus __not_in_flash_func(status)() const;
	size_t printTo(Print &p) const override;

private:
	friend void __not_in_flash_func(c_gpio_irq_handler)(unsigned int gpio, uint32_t event_mask);
	void __not_in_flash_func(gpio_irq_handler)(unsigned int gpio, uint32_t event_mask);

	const unsigned int gpio_;
	const bool active_low_;

	bool on_ = false;
	uint64_t start_us_ = 0;
	uint64_t time_count_us_ = 0;
	uint64_t gpio_change_count_ = 0;
	uint64_t gpio_irq_count_ = 0;
	spin_lock_t *lock_ = nullptr;
};
