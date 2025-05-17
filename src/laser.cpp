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

#include "laser.h"

#include <Arduino.h>
#include <hardware/gpio.h>
#include <hardware/sync.h>

static LaserTime *instance = nullptr;

LaserTime::LaserTime(unsigned int gpio, bool active_low)
		: gpio_(gpio), active_low_(active_low) {
}

void __not_in_flash_func(c_gpio_irq_handler)(unsigned int gpio, uint32_t event_mask) {
	instance->gpio_irq_handler(gpio, event_mask);
}

void LaserTime::start() {
	lock_ = spin_lock_init(spin_lock_claim_unused(true));

	_gpio_init(LED_BUILTIN);
	gpio_set_dir(LED_BUILTIN, GPIO_OUT);
	gpio_put(LED_BUILTIN, false);

	_gpio_init(gpio_);
	gpio_set_dir(gpio_, GPIO_IN);
	gpio_set_pulls(gpio_, active_low_, !active_low_);

	instance = this;
	gpio_set_irq_enabled_with_callback(gpio_,
		GPIO_IRQ_LEVEL_LOW | GPIO_IRQ_LEVEL_HIGH, true,
		&c_gpio_irq_handler);
}

uint64_t __not_in_flash_func(LaserTime::uptime_us)() {
	return to_us_since_boot(get_absolute_time());
}

LaserStatus __not_in_flash_func(LaserTime::status)() const {
	LaserStatus status;
	uint64_t start_us;
	auto saved_irq = spin_lock_blocking(lock_);

	start_us = start_us_;
	status.total_time_us = time_count_us_;
	status.on = on_;
	spin_unlock(lock_, saved_irq);

	if (status.on) {
		status.current_time_us = uptime_us() - start_us;
		status.total_time_us += status.current_time_us;
	} else {
		status.current_time_us = 0;
	}

	return status;
}

void __not_in_flash_func(LaserTime::gpio_irq_handler)(unsigned int gpio, uint32_t event_mask) {
	if (gpio == gpio_) {
		uint64_t now_us = uptime_us();
		bool raw_value = gpio_get(gpio);
		bool value = raw_value ^ active_low_;
		auto saved_irq = spin_lock_blocking(lock_);

		if (on_ != value) {
			if (on_) {
				time_count_us_ += now_us - start_us_;
			}

			start_us_ = now_us;
			on_ = value;
			spin_unlock(lock_, saved_irq);

			gpio_change_count_++;
			gpio_put(LED_BUILTIN, on_);
		} else {
			spin_unlock(lock_, saved_irq);
		}

		gpio_irq_count_++;

		gpio_set_irq_enabled(gpio,
			raw_value ? GPIO_IRQ_LEVEL_HIGH : GPIO_IRQ_LEVEL_LOW, false);
		gpio_set_irq_enabled(gpio,
			raw_value ? GPIO_IRQ_LEVEL_LOW : GPIO_IRQ_LEVEL_HIGH, true);
	} else {
		gpio_set_irq_enabled(gpio, event_mask, false);
	}
}

size_t LaserTime::printTo(Print &p) const {
	LaserStatus status = this->status();
	size_t len = 0;

	len += p.print("{state: ");
	len += p.print(status.on);
	len += p.print(", changes: ");
	len += p.print(gpio_change_count_);
	len += p.print(", interrupts: ");
	len += p.print(gpio_irq_count_);
	len += p.print(", total_time_us: ");
	len += p.print(status.total_time_us);
	len += p.print(", current_time_us: ");
	len += p.print(status.current_time_us);
	len += p.print("}");

	return len;
}
