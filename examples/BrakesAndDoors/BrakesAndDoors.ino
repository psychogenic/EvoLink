/*
 * BrakesAndDoors.ino -- EvoLink brake and door events
 * example, part of the cross-platform EVO-All
 * interface library.
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
 *
 * Here, we use the serial port to debug/view brake/door event
 * reports and a SoftwareSerial to communicate with the
 * EVO-All--though in real life, I'd recommend using the
 * hardware serial to talk to EVO-All.
 *
 * To do so, we create and assign a couple of event notification
 * callbacks, and just
 * 	EVO.checkActivity()
 * all the time, in the main loop.
 *
 * Be sure to configure your system according to the
 * various defines below (port/baud rates).
 */



#include <SoftwareSerial.h>
#include <EvoLink.h>

// specify that we'll be using the
// EvoLink namespace -- without this line, pretty
// much every type used would need a prefix, e.g.
// EvoLink::Brake::Off rather than just Brake::Off.
using namespace EvoLink;

SoftwareSerial evoSerial(10, 11);

#define DEBUG_SERIAL					Serial
#define DEBUG_BAUD_RATE					14400

#define EVO_SERIAL_DEVICE_PTR			&evoSerial
#define EVO_BAUD_RATE					9600

/*
 * First, we create a few callback functions of the correct types:
 *
 * One that receives Brake::Event notifications, the other to catch
 * OpenClose::Events.
 *
 * These examples do nothing but print out a notice, but you can
 * of course do whatever you like when something of interest happens.
 *
 */
void brake_event_received(Brake::Event brake_event)
{
  switch (brake_event)
  {
    case Brake::On:
      DEBUG_SERIAL.println(F("Brake ON"));
      break;
    case Brake::Off:
      DEBUG_SERIAL.println(F("Brake off"));
      break;

    case Brake::HandBrake_On:
      DEBUG_SERIAL.println(F("Hand break ON"));
      break;
    case Brake::HandBrake_Off:
      DEBUG_SERIAL.println(F("Hand break off"));
      break;
  }

}

// callback for the open/close events...
void openclose_event_received(OpenClose::Event event)
{
  switch (event)
  {
    case OpenClose::Door_Opened:
      DEBUG_SERIAL.println(F("Door OPENED"));
      break;

    case OpenClose::Door_Closed:
      DEBUG_SERIAL.println(F("Door closed"));
      break;

    case OpenClose::Trunk_Opened:
      DEBUG_SERIAL.println(F("Trunk OPENED"));
      break;

    case OpenClose::Trunk_Closed:
      DEBUG_SERIAL.println(F("Trunk closed"));
      break;

    case OpenClose::Hood_Opened:
      DEBUG_SERIAL.println(F("Hood OPENED"));
      break;

    case OpenClose::Hood_Closed:
      DEBUG_SERIAL.println(F("Hood closed"));
      break;
  }

}


/*
 * Standard setup().
 *
 * We'll setup the debug serial output, tell EvoLink about the
 * callbacks we made and call EVO.begin() to get going.
 */
void setup()
{
  // setup our output serial
  DEBUG_SERIAL.begin(DEBUG_BAUD_RATE);

  // Tell EVO about the callbacks we created above:
  EVO.callbacks.brake_event = brake_event_received;
  EVO.callbacks.openclose_event = openclose_event_received;


  // setup EVO to user the serial device, at specified baud rate, and
  // to call begin(EVO_BAUD_RATE) for us:
  EVO.begin(SerialSetup((HardwareSerial*)EVO_SERIAL_DEVICE_PTR, EVO_BAUD_RATE, true));

  DEBUG_SERIAL.println(F("Setup complete -- will now monitor events..."));

}


void loop()
{
  // here, we'll do nothing but let EvoLink look for, and
  // react to, event notifications.  Any brake or open/close
  // event will trigger the appropriate callback.
  EVO.checkActivity();
}