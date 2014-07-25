/*
 * Basic.ino -- Basic EvoLink example,
 * part of the cross-platform EVO-All interface library.
 *
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
 * This is a very basic example of EvoLink usage.  It demonstrates
 * a simple setup and will simply lock/unlock the vehicle repeatedly.
 */

#include <EvoLink.h>

using namespace EvoLink;

#define EVO_SERIAL_DEVICE_PTR			&Serial
#define EVO_BAUD_RATE				9600

void setup()
{

  // setup EVO to user the serial device, at specified baud rate, and
  // to call begin(EVO_BAUD_RATE) for us:
  EVO.begin(SerialSetup((HardwareSerial*)EVO_SERIAL_DEVICE_PTR, EVO_BAUD_RATE, true));

}

int counter = 0;
void loop()
{

  // may want to make a habit of
  // regularly calling checkActivity() to
  // handle any incoming messages--this will
  // be useful later in more advanced scenarios
  EVO.checkActivity();

  if (counter++ % 2 == 0)
  {
    // perform a lock dance...
    EVO.wakeUp();
    EVO.parklightOn();
    EVO.starterKillOn();
    EVO.systemArm();
    EVO.lock();
    delay(750); // wait a bit
    EVO.parklightOff();

  } else {
    // this time, we unlock!
    EVO.wakeUp();
    EVO.alarmOff();
    EVO.starterKillOff();
    EVO.parklightOn();
    EVO.systemDisarm();
    EVO.unlock();
    EVO.parklightOff();
    delay(250);
    EVO.parklightOn();
    delay(250);
    EVO.parklightOff();
  }

  // wait a bit until we loop again...
  // By using delayWhileCheckingActivity(), we ensure any
  // incoming EVO messages will be handled as we wait for the
  // delay to expire.
  EVO.delayWhileCheckingActivity(2000);
}
