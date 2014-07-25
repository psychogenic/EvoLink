/*
 * CustomCallbacks.ino -- EvoLink event callbacks
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
 * Here, we use the serial port to debug/view reports and
 * a SoftwareSerial to communicate with the EVO-All--though
 * in real life, I'd recommend using the hardware serial to
 * talk to EVO-All.
 *
 * To do so, we create and assign a event notification
 * callbacks for all types of evets.  Then, we'll periodically
 * issue different commands and check for any activity from
 * EVO-All.
 *
 * There's also an example of using the "input request" status
 * byte, that shows how the system reports various states (as
 * per the Fortin recommendation, don't abuse this request
 * in the real world--it's better to use callbacks and track
 * the states as they change).
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
 * Here we'll create a couple of "custom callbacks" to
 * override the default processing of received messages.
 *
 * Each custom callback has the same signature, and can
 * be assigned such that it will be triggered for one, or
 * more, datalink messages.
 *
 */


/*
 * my_custom_brakeOn_cb will be triggered only by
 * brake ON events (see setup())
 */
void my_custom_brakeOn_cb(DataLink::MessageCode message)
{
  // we don't care about the message here--can only be DataLink::Brake_On

  DEBUG_SERIAL.println(F("Brakes activated!"));

}

/*
 * my_custom_door_events_cb will be assigned to door open and close
 * events, so we need to check which message we actually received
 * to act appropriately. See setup().
 *
 */
void my_custom_door_events_cb(DataLink::MessageCode message)
{

  DEBUG_SERIAL.print(F("Door... "));

  // note: message codes received are in EvoLink::DataLink namespace
  switch (message)
  {
    case DataLink::Door_Opened:
      DEBUG_SERIAL.println(F("OPENED!"));
      break;

    case DataLink::Door_Closed:
      DEBUG_SERIAL.println(F("closed."));
      break;

    default:
      // we shouldn't get here!
      DEBUG_SERIAL.println(F("uh... something weird going on."));
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
  DEBUG_SERIAL.begin(DEBUG_BAUD_RATE);

  // We won't be using any of the type-specific callbacks supported
  // by the system (such as in the CallbackSystem example) but will
  // override only those for messages of interest,
  // using setHandlerForMessage()

  // brake on:
  EVO.setHandlerForMessage(DataLink::Brake_On, my_custom_brakeOn_cb);

  // both the door open and door close messages will trigger my_custom_door_events_cb()
  EVO.setHandlerForMessage(DataLink::Door_Opened, my_custom_door_events_cb);
  EVO.setHandlerForMessage(DataLink::Door_Closed, my_custom_door_events_cb);

  // because we aren't using any standard callbacks, and have only assigned custom
  // callbacks for 3 message types, all other messages received will simply
  // be dropped (like it's hot).


  // setup EVO to user the serial device, at specified baud rate, and
  // to call begin(EVO_BAUD_RATE) for us:
  EVO.begin(SerialSetup((HardwareSerial*)EVO_SERIAL_DEVICE_PTR, EVO_BAUD_RATE, true));

  DEBUG_SERIAL.println(F("Setup complete -- will now monitor events..."));

}

// setup a global counter, so we'll do something different
// on every pass through the loop...
uint8_t counter = 0;


void loop()
{

  // here we'll just do some busy work--in reality we're just waiting
  // for the brake and door messages to trigger our callbacks...

  counter++; // increment our counter on each loop

  EVO.wakeUp(); // always wake-up before messaging

  switch (counter)
  {
    case 1:
      EVO.lock(Driver::One);
      EVO.parklightOff();
      EVO.horn(Horn::Beep50ms);
      break;

    case 2:
      EVO.unlock(Driver::One);
      EVO.parklightOn();
      break;

    default:
      counter = 0; // reset our counter
  }

  // we need to call checkActivity to get and handle
  // any incoming messages.  Here, we'll set the timeout
  // parameter, such that EvoLink will wait (up to) 300ms
  // for some message(s) to handle.
  EVO.checkActivity(300);
}