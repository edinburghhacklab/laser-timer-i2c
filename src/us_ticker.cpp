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

#include <us_ticker_api.h>
#include <mbed_critical.h>
#include <hardware/timer.h>

extern "C" {

static const uint8_t alarm_num = 0;
static uint64_t last_read_u64 = 0;

uint32_t __real_us_ticker_read();
uint32_t __wrap_us_ticker_read() {
	uint64_t now_u64 = time_us_64();

	core_util_critical_section_enter();
	last_read_u64 = now_u64;
	core_util_critical_section_exit();

	return now_u64;
}

void __real_us_ticker_set_interrupt(timestamp_t timestamp_u32);
void __wrap_us_ticker_set_interrupt(timestamp_t timestamp_u32) {
	core_util_critical_section_enter();

	uint32_t last_read_u32 = last_read_u64;
	uint64_t timestamp_u64 = timestamp_u32 | (last_read_u64 & 0xFFFFFFFF00000000ULL);

	if (timestamp_u32 < last_read_u32) {
		timestamp_u64 += 1ULL << 32;
	}

	absolute_time_t target = { timestamp_u64 };
	bool missed = hardware_alarm_set_target(alarm_num, target);

	core_util_critical_section_exit();

	if (missed) {
		us_ticker_fire_interrupt();
	}
}

} // extern "C"
