/*
 * config.h -- EvoLink compile-time config, part of the
 * cross-platform EVO-All interface library.
 * Copyright (C) 2014 Pat Deegan. All Rights Reserved.
 *
 * http://flyingcarsandstuff.com/projects/EvoLink
 *
 * Please let me know if you use EvoLink in your projects, and
 * provide a URL if you'd like me to link to it from the EvoLink
 * home.
 *
 * Released under the GPL v3, dual licensing available.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * ************************* OVERVIEW *************************
 *
 * This file contains various compile-time switches and settings.
 *
 */

#ifndef EVOLINK_CONFIG_H_
#define EVOLINK_CONFIG_H_

#define EVOLINK_VERSION_MAJOR		1
#define EVOLINK_VERSION_MINOR		0

// PLATFORM_XXX
// Define *one of* the available platforms, for which you
// are compiling.  Currently, only PLATFORM_ARDUINO supported.
#define PLATFORM_ARDUINO


// Default baudrate specified by BAUDRATE_DEFAULT
#define BAUDRATE_DEFAULT								9600

#define ABS_REQUEST_RESPONSE_TIME_MINIMUM_MS			1
//#define ABS_REQUEST_RESPONSE_TIME_MINIMUM_MS			104

// AUTODELAY_DEFAULT_MS minimum delay before transmitting
// commands/requests on the serial line.
#define AUTODELAY_DEFAULT_MS							50

// POSTWAKEUP_AUTO_DELAY_MS extra delay to insert after
// sending a wakeup command, to ensure the EVO-All is
// up and running.
#define POSTWAKEUP_AUTO_DELAY_MS						90

// POSTGROUNDOUT_ON_DELAY_MS extra delay to insert after
// sending a ground-out ON command, to ensure EVO-All is
// ready to go.
#define POSTGROUNDOUT_ON_DELAY_MS						100

// if you define MINTIME_BETWEEN_WAKEUPS_MS, wakeUp will not
// be repeated within MINTIME_BETWEEN_WAKEUPS_MS ms.  So if you
// call
//  EVO.wakeUp(); // wake-up command issued
//	EVO.XYZ();
//  EVO.wakeUp(); // to soon, wake-up command ignored.
#define MINTIME_BETWEEN_WAKEUPS_MS						1100


#define SYNCHRONOUS_GETTER_DEFAULT_TIMEOUT_MS			ABS_REQUEST_RESPONSE_TIME_MINIMUM_MS * 3

// define AUTO_CHECKACTIVITY_BEFORE_REQUESTS to
// automatically do a checkActivity() before any
// request/command.
#define AUTO_CHECKACTIVITY_BEFORE_REQUESTS


// Define DEBUG_USART_ENABLE (and set SerialSetup param
// accordingly) to enable debug output on usart/serial).
// #define DEBUG_USART_ENABLE

#endif /* EVOLINK_CONFIG_H_ */
