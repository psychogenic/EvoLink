/*
 * EvoLink.h -- Cross-platform EVO-All interface library.
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
 * The EVO-All devices are easy to interface with--a simple 4-wire
 * connector (for power and UART (serial) connectivity) and you're
 * ready to go.  However, dealing with the various byte codes and
 * timing issues can be difficult, so EvoLink handles all this for
 * you.
 *
 * This example uses the setup()/loop() system used by Arduino, but
 * the idea would be the same on any platform.
 *
 *  // In the setup, we give the EVO global information about the
 *  // SerialSetup to use (SerialSetup is a platform-specific object
 *  // that encapsulates USART info for EVOLink).
 *	void setup()
 *	{
 *
 *		// setup EVO to user the serial device, at specified baud rate, and
 *		// to call begin(EVO_BAUD_RATE) for us:
 *		EVO.begin(SerialSetup((HardwareSerial*)&Serial));
 *
 *	}
 *
 *
 *	// During operation, we *must* do one thing, i.e. call:
 *	// 		EVO.checkActivity()
 *	// regularly, and *may* do other things, by calling the
 *	// various methods.
 *	void loop()
 *	{
 *
 *		// may want to make a habit of
 *		// regularly calling checkActivity() to
 *		// handle any incoming messages--this will
 *		// be useful later in more advanced scenarios
 *		EVO.checkActivity();
 *
 *		if (some_condition())
 *		{
 *			// let's unlock the doors, say.
 *
 *			// always wake up, before a sequence of commands
 *			EVO.wakeUp();
 *
 *			// do unlock-door related stuff...
 *			EVO.alarmOff();
 *			EVO.starterKillOff();
 *			EVO.parklightOn();
 *			EVO.systemDisarm();
 *			EVO.unlock(); // actually unlock the doors! :)
 *
 *			delay(400); // leave the lights on a bit...
 *
 *			EVO.parklightOff();
 *		}
 *
 *	} // end main loop()
 *
 *	See the included examples and documentation, online at
 *		http://flyingcarsandstuff.com/projects/EvoLink
 *	for further information.
 */

#ifndef EVOLINK_MAIN_INCLUDE_H_
#define EVOLINK_MAIN_INCLUDE_H_

#include "includes/dependencies.h"
#include "includes/driver.h"



#endif /* EVOLINK_H_ */
