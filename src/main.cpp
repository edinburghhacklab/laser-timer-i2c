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
#include <hardware/gpio.h>
#include <hardware/sync.h>

static constexpr unsigned int LASER_GPIO = 22;
static constexpr bool LASER_ACTIVE_LOW = true;

static bool laser_on = false;
static uint64_t laser_start_us = 0;
static uint64_t laser_time_count_us = 0;
static uint64_t laser_gpio_change_count = 0;
static uint64_t laser_gpio_irq_count = 0;
static spin_lock_t *lock = nullptr;

static void led(bool on) {
	gpio_put(LED_BUILTIN, on);
}

static uint64_t uptime_us() {
	return to_us_since_boot(get_absolute_time());
}

static uint64_t laser_time_us() {
	auto saved_irq = spin_lock_blocking(lock);
	uint64_t time_us = laser_time_count_us;

	if (laser_on) {
		time_us += uptime_us() - laser_start_us;
	}

	spin_unlock(lock, saved_irq);
	return time_us;
}

static void gpio_irq_handler(unsigned int gpio, uint32_t event_mask) {
	if (gpio == LASER_GPIO) {
		auto saved_irq = spin_lock_blocking(lock);
		uint64_t now_us = uptime_us();
		bool raw_value = gpio_get(gpio);
		bool value = raw_value ^ LASER_ACTIVE_LOW;

		if (laser_on != value) {
			if (laser_on) {
				laser_time_count_us += now_us - laser_start_us;
			}

			laser_start_us = now_us;
			laser_on = value;
			laser_gpio_change_count++;
			led(laser_on);
		}

		laser_gpio_irq_count++;
		spin_unlock(lock, saved_irq);

		gpio_set_irq_enabled(gpio,
			raw_value ? GPIO_IRQ_LEVEL_HIGH : GPIO_IRQ_LEVEL_LOW, false);
		gpio_set_irq_enabled(gpio,
			raw_value ? GPIO_IRQ_LEVEL_LOW : GPIO_IRQ_LEVEL_HIGH, true);
	} else {
		gpio_set_irq_enabled(gpio, event_mask, false);
	}
}

void setup() {
	lock = spin_lock_init(spin_lock_claim_unused(true));

	_gpio_init(LED_BUILTIN);
	gpio_set_dir(LED_BUILTIN, GPIO_OUT);
	led(false);

	_gpio_init(LASER_GPIO);
	gpio_set_dir(LASER_GPIO, GPIO_IN);
	gpio_set_pulls(LASER_GPIO, LASER_ACTIVE_LOW, !LASER_ACTIVE_LOW);

	gpio_set_irq_enabled_with_callback(LASER_GPIO,
		GPIO_IRQ_LEVEL_LOW | GPIO_IRQ_LEVEL_HIGH,
		true, gpio_irq_handler);
}

void loop() {
	Serial.print("{uptime: ");
	Serial.print(uptime_us());
	Serial.print(", state: ");
	Serial.print(laser_on);
	Serial.print(", changes: ");
	Serial.print(laser_gpio_change_count);
	Serial.print(", interrupts: ");
	Serial.print(laser_gpio_irq_count);
	Serial.print(", time_us: ");
	Serial.print(laser_time_us());
	Serial.println("}");
	sleep_ms(1000);
}
