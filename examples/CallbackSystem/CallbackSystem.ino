/*
 * CallbackSystem.ino -- EvoLink event callbacks
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
 * First, we create a callback function of every supported type.
 *
 * Each will handle all the relevant cases it may encounter, though
 * your own code may only be interested in a subset of possible events.
 *
 * Most receive function-specific messages, e.g. Brake or Tach
 * events.
 *
 * These examples do nothing but print out a notice, but you can
 * of course do whatever you like when something of interest happens.
 *
 */

// Brake-related events
void brake_event_received(Brake::Event brake_event) {

  DEBUG_SERIAL.println(' ');
  switch (brake_event) {
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

// Open/close events (doors, trunk, etc)
void openclose_event_received(OpenClose::Event event) {
  DEBUG_SERIAL.println(' ');
  switch (event) {
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

// Various remote starter events
void remstarter_event_received(RemoteStarter::Event event) {
  DEBUG_SERIAL.println(' ');
  switch (event) {

    case RemoteStarter::Disarm:
      DEBUG_SERIAL.println(F("RemStart: Disarm"));
      break;

    case RemoteStarter::Arm:
      DEBUG_SERIAL.println(F("RemStart: Arm"));
      break;
    case RemoteStarter::On:
      DEBUG_SERIAL.println(F("RemStart: On"));
      break;
    case RemoteStarter::Off:
      DEBUG_SERIAL.println(F("RemStart: Off"));
      break;
    case RemoteStarter::UnlockDisarm:
      DEBUG_SERIAL.println(F("RemStart: UnlockDisarm"));
      break;

    case RemoteStarter::LockArm:
      DEBUG_SERIAL.println(F("RemStart: LockArm"));
      break;

  }

}

// Sensing-related events (shocks, tilt, speed)
void sensor_event_received(Sensor::Event event) {
  DEBUG_SERIAL.println(' ');
  switch (event) {
    case Sensor::Shock:
      DEBUG_SERIAL.println(F("Sensor: Shock"));
      break;
    case Sensor::Alarm_PreWarn:
      DEBUG_SERIAL.println(F("Sensor: Alarm_PreWarn"));
      break;
    case Sensor::Tilt:
      DEBUG_SERIAL.println(F("Sensor: Tilt"));
      break;
    case Sensor::Over_15MPH:
      DEBUG_SERIAL.println(F("Sensor: Over_15MPH"));
      break;

  }
}

// tachometer events
void tach_event_received(Tach::Event event) {

  DEBUG_SERIAL.println(' ');
  switch (event) {
    case Tach::On:
      DEBUG_SERIAL.println(F("Tach: On"));
      break;
    case Tach::Off:
      DEBUG_SERIAL.println(F("Tach: Off"));
      break;
    case Tach::OverRev:
      DEBUG_SERIAL.println(F("Tach: OverRev"));
      break;
  }
}

// miscellaneous messages that may be received but aren't
// covered by a function-specific callback
void misc_message_received(DataLink::MessageCode message) {
  DEBUG_SERIAL.println(' ');
  switch (message) {
    case DataLink::Ping_Message:
      DEBUG_SERIAL.println(F("MSG: Ping"));
      break;

    case DataLink::Pong_Message:
      DEBUG_SERIAL.println(F("MSG: Pong"));
      break;
    case DataLink::CarKey_In_On:
      DEBUG_SERIAL.println(F("MSG: Car key in on"));
      break;
    case DataLink::CarKey_In_Off:
      DEBUG_SERIAL.println(F("MSG: Car key in off"));
      break;
    case DataLink::RemoteProgramming_Enable:
      DEBUG_SERIAL.println(F("MSG: RemProg Enable"));
      break;
    case DataLink::RemoteProgramming_Disable:
      DEBUG_SERIAL.println(F("MSG: RemProg Disable"));
      break;
    default:
      DEBUG_SERIAL.print(F("MSG: Unknown value"));
      DEBUG_SERIAL.println((int) (message));
      break;

  }

}

/*
 * Data responses.  Some commands are requests for more information.  You may
 * issue the request at any point (e.g. EVO.requestVSS()) and, when the data
 * response is received later, the requested_data_received callback will be triggered.
 *
 * When called, this callback gets:
 *
 * 	* the RequestCode that triggered the response (e.g. Request_VSS)
 * 	* the value returned by the module
 *
 * NOTE: the value for some requests is *encoded* in various ways. EvoLink handles
 * decoding this value--your callback gets ACTUAL temperature/tach readings etc,
 * so you have no further processing to do.
 *
 * There is a second method of doing this--the synchronous getXXX() methods.  These
 * will bypass the callback mechanism and allow for a more direct mechanism of access:
 *
 * int16_t temp = EVO.getTemperature();
 *
 * See the docs for details.
 *
 */
void dataforrequest_received(DataLink::RequestCode request, int valueReturned) {
  DEBUG_SERIAL.println(' ');
  EvoLink::InputStatus inpStat(valueReturned);
  switch (request) {
    case DataLink::Request_VSS:
      DEBUG_SERIAL.print(F("VSS data: "));
      DEBUG_SERIAL.println(valueReturned);
      break;

    case DataLink::Request_Tach:
      DEBUG_SERIAL.print(F("Tach data: "));
      DEBUG_SERIAL.println(valueReturned);
      break;

    case DataLink::Request_Temperature:
      DEBUG_SERIAL.print(F("Temp data: "));
      DEBUG_SERIAL.println(valueReturned);
      break;

    case DataLink::Request_Input:
      DEBUG_SERIAL.print(F("Input data: "));
      DEBUG_SERIAL.println(valueReturned);
      DEBUG_SERIAL.print(F("\tBrake:\t"));
      DEBUG_SERIAL.println((inpStat.brake == State::On) ? F("ON") : F("OFF"));
      DEBUG_SERIAL.print(F("\tHBrake:\t"));
      DEBUG_SERIAL.println(
        (inpStat.hand_brake == State::On) ? F("ON") : F("OFF"));
      DEBUG_SERIAL.print(F("\tDoor:\t"));
      DEBUG_SERIAL.println(
        (inpStat.door == State::Open) ? F("Open") : F("Closed"));
      DEBUG_SERIAL.print(F("\tTrunk:\t"));
      DEBUG_SERIAL.println(
        (inpStat.trunk == State::Open) ? F("Open") : F("Closed"));
      DEBUG_SERIAL.print(F("\tHood:\t"));
      DEBUG_SERIAL.println(
        (inpStat.hood == State::Open) ? F("Open") : F("Closed"));
      DEBUG_SERIAL.print(F("\tTach:\t"));
      DEBUG_SERIAL.println((inpStat.tach == State::On) ? F("ON") : F("OFF"));
      DEBUG_SERIAL.println('\n');

      break;

    default:
      DEBUG_SERIAL.println(F("Data received for unknown request"));
      break;
  }
}

/*
 * Various errors that you may want to be aware of--this callback
 * receives the errors--additional info may be passed in the more_info
 * parameter (see the docs).
 */
void error_event(ErrorMessage::Event event, uint8_t more_info) {

  DEBUG_SERIAL.println(' ');
  switch (event) {
    case ErrorMessage::UnsupportedValue:
      DEBUG_SERIAL.print(F("ERROR: Unsupported value received -- "));
      DEBUG_SERIAL.println(more_info, HEX);
      break;
    case ErrorMessage::RequestTimeout:
      DEBUG_SERIAL.print(F("ERROR: Timeout for request -- "));
      DEBUG_SERIAL.println(more_info, HEX);

      break;
    case ErrorMessage::Temperature_Error:
      DEBUG_SERIAL.println(F("ERROR: Temp read error or no thermo present"));
      break;

  }
}

/*
 * Standard setup().
 *
 * We'll setup the debug serial output, tell EvoLink about the
 * callbacks we made and call EVO.begin() to get going.
 */
void setup() {
  (EVO_SERIAL_DEVICE_PTR)->begin(EVO_BAUD_RATE);
  DEBUG_SERIAL.begin(DEBUG_BAUD_RATE);

  // Tell EVO about the callbacks we created above:
  EVO.callbacks.brake_event = brake_event_received;
  EVO.callbacks.error_event = error_event;
  EVO.callbacks.message_received = misc_message_received;
  EVO.callbacks.openclose_event = openclose_event_received;
  EVO.callbacks.remotestarter_event = remstarter_event_received;
  EVO.callbacks.requested_data_received = dataforrequest_received;
  EVO.callbacks.sensor_event = sensor_event_received;
  EVO.callbacks.tach_event = tach_event_received;

  // setup EVO to user the serial device, at specified baud rate, and
  // to call begin(EVO_BAUD_RATE) for us:
  SerialSetup ss((HardwareSerial*) EVO_SERIAL_DEVICE_PTR, EVO_BAUD_RATE,
                 true);
  // ss.debug_usart = &(DEBUG_SERIAL);
  EVO.begin(ss);
  EVO.setAutoDelayMs(20);

  DEBUG_SERIAL.println(F("Setup complete -- will now monitor events..."));

}

// setup a global counter, so we'll do something different
// on every pass through the loop...
uint8_t counter = 0;

void loop() {

  counter++; // increment our counter on each loop

  switch (counter) {
    case 1:
      DEBUG_SERIAL.print('T');

      EVO.wakeUp(); // always wake-up before messaging
      EVO.requestTemperature(); // request the temp be sent to us
      break;

    case 2:
      DEBUG_SERIAL.print('L');
      EVO.wakeUp(); // always wake-up before messaging
      EVO.ping();
      EVO.lock(Driver::One);
      EVO.parklightOff();
      EVO.horn(Horn::Beep50ms);
      break;

    case 3:
      DEBUG_SERIAL.print('S');
      EVO.wakeUp(); // always wake-up before messaging
      EVO.requestStatus(); // request the status byte be sent to us
      break;

    case 4:
      DEBUG_SERIAL.print('U');
      EVO.wakeUp(); // always wake-up before messaging
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

  DEBUG_SERIAL.print('.');
  // do a 2.5 second delay, while checking for activity
  // every 150 ms.
  EVO.delayWhileCheckingActivity(2500, 150);
}