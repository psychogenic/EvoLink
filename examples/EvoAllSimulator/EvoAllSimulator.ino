/*
 * EvoAllSimulator.ino -- Evo-All simulator,
 * part of the cross-platform EvoLink interface library.
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
 * This code does NOT really make use of the EvoLink
 * library--it is used as a simple simulator for the
 * remote side, i.e. the EVO-ALL device itself.
 *
 * I use this to test EvoLink related functionality.
 * If you connect to the simulator through the Serial
 * Monitor (or through Druid4Arduino), you can trigger
 * events manually.  Otherwise, the simulator will
 * respond to requests and randomly emit messages at
 * fixed intervals.
 *
 * To get it working you build this code and run it on
 * an Arduino ("simulator") and hook it up to a second
 * board ("test", i.e. whatever you're developing),
 * through the UART:
 *
 *
 *
 *                 Simulator              Test
 *             +=============+       +=============+
 *  ^          |             |       |             |
 *  |_R_/ _____|Brake        |       |             |
 *       =     |             |       |             |
 *      _|_    |             |       |             |
 *      GND    |             |       |             |
 *             |           Rx|<------|Tx           |
 *  ^          |           Tx|------>|Rx           |
 *  |_R_/ _____|Door         |       |             |
 *       =     |             |       |             |
 *      _|_    |             |       |             |
 *      GND    |             |       |             |
 *             |             |       |             |
 *  ^          |             |       |             |
 *  |_R_/ _____|Hood         |       |             |
 *       =     |             |       |             |
 *      _|_    |             |       |             |
 *      GND    |Horn Dr  Alrm|       |             |
 *             +=============+       +=============+
 *               |   |    |
 *               R   R    R
 *               |   |    |
 *               L   L    L
 *              _|_ _|_  _|_
 *              GND GND  GND
 *
 * All external components are optional, really, but it's nice to have
 * a way to manually activate some stuff so there are
 *
 *  * Three switched inputs -- ideally use an RC network to debounce,
 *    and feed buttons to inputs defined below; and
 *
 *  * Three LED outputs -- tie resistor+LED to each of Horn/Door/Alarm
 *    outputs defined below.
 *
 */

// Uncomment the USING_PLATFORM_XMEGA if you
// are using an XMEGA and the Xmegaduino system
// to build the software (Xmega has a bunch of
// hardware serial ports to play with and is
// pretty cool!).

// #define USING_PLATFORM_XMEGA


#include <EvoLink.h>
#include <SerialUI.h>
#ifndef USING_PLATFORM_XMEGA
#include <SoftwareSerial.h>
#endif




/*
 * Define the pins used for various input/outputs
 */
#define brake_input_pin		5
#define door_input_pin		6
#define hood_input_pin		7

#define horn_output_pin		8
#define door_output_pin		9
#define alarm_output_pin	12


/*
 * Define the interval, in ms, at which random
 * messages are issued.
 */
#define random_msg_interval_ms				500


#define time_until_wakeup_required_default	300
#define min_delay_between_requests_ms		10

#define horn_on_time_multiplier				3

#define datarequest_response_delay_ms		110




/*
 * Enable various typesof random message output by
 * defining the related OUTPUT_RANDOM_EVENTS_XYZ
 */
#define OUTPUT_RANDOM_EVENTS_REMSTART
#define OUTPUT_RANDOM_EVENTS_OPENCLOSE
#define OUTPUT_RANDOM_EVENTS_BRAKES
#define OUTPUT_RANDOM_EVENTS_SENSOR
#define OUTPUT_RANDOM_EVENTS_TACH
#define OUTPUT_RANDOM_EVENTS_MISC

/*
 * CONSISTENT_STATE_RANDOM_EVENTS
 *
 * Defining CONSISTENT_STATE_RANDOM_EVENTS ensures that
 * randomly generated events make sense -- e.g. you'll
 * never get two consecutive "brake on" messages... when
 * brakes are "on" the only related message you may receive
 * is "brake off", same for hand brake and
 * hood, trunk etc. open/close.
 *
 */
#define CONSISTENT_STATE_RANDOM_EVENTS


#define OUTPUT_DEBUG_INFO

/*
 * RANDOMIZE_MSGOUTPUT_BASED_ON_REQ --
 * random seed will be set by time of first incoming request message, if defined.
 * Good if you want random messages, bad if you want something predictable.
 */
#define RANDOMIZE_MSGOUTPUT_BASED_ON_REQ


SUI_DeclareString(device_greeting,
                  "+++ Welcome to the Evo Simulator +++\r\nEnter '?' to list available options.");

SUI_DeclareString(top_menu_title, "EvoSim Main Menu");


SUI_DeclareString(toggle_rnd_events, "random");
SUI_DeclareString(toggle_rnd_events_help, "Toggle auto events");


SUI_DeclareString(toggle_input_mon, "inputs");
SUI_DeclareString(toggle_input_mon_help, "Toggle input monitoring");


SUI_DeclareString(gen_rnd_events, "generate");
SUI_DeclareString(gen_rnd_events_help, "Generate a random event");


SUI_DeclareString(send_code, "send");
SUI_DeclareString(send_code_help, "Send a specific code");


SUI_DeclareString(door_toggle, "door");
SUI_DeclareString(door_toggle_help, "Open/Close door");


SUI_DeclareString(hood_toggle, "hood");
SUI_DeclareString(hood_toggle_help, "Open/Close hood");

SUI_DeclareString(brake_toggle, "brake");
SUI_DeclareString(brake_toggle_help, "Apply/release brake");


SUI_DeclareString(sim_rcv, "receive");
SUI_DeclareString(sim_rcv_help, "Simulate command reception");

SUI_DeclareString(wake_up_cmd, "wake");
SUI_DeclareString(wake_up_cmd_help, "Simulate wake-up reception");


SUI_DeclareString(msg_input_action, "Input change detected");
SUI_DeclareString(error_cant_add, "Could not addCommand to mainMenu?");

SUI_DeclareString(wakeup_timeout, "timeout");
SUI_DeclareString(wakeup_timeout_help, "Set max time between wake-ups/requests");

SUI_DeclareString(status_req, "status");
SUI_DeclareString(status_req_help, "Simulate status request");

#if not (defined(SERIALUI_VERSION_AT_LEAST) and SERIALUI_VERSION_AT_LEAST(1, 8))
#error "You need to install SerialUI version 1.8 (or later) from http://flyingcarsandstuff.com/"
#endif





// specify that we'll be using the
// EvoLink namespace -- without this line, pretty
// much every type used would need a prefix, e.g.
// EvoLink::Brake::Off rather than just Brake::Off.
using namespace EvoLink;


#define DEBUG_BAUD_RATE					115200
#define EVO_BAUD_RATE					9600

#ifdef USING_PLATFORM_XMEGA
// if using XMEGA, we have a bunch of hardware
// serials to choose from:
#define DEBUG_SERIAL					Serial3
#define EVO_SERIAL_DEVICE_PTR			(&Serial)
#else
// regular arduino with only 1 hardware serial port...
// create a SoftwareSerial on 10/11:
SoftwareSerial evoSerial(10, 11);
#define DEBUG_SERIAL					Serial
#define EVO_SERIAL_DEVICE_PTR			(&evoSerial)
#endif



// our global SerialUI instance:
SUI::SerialUI mySUI(device_greeting, 5, &(DEBUG_SERIAL));



// global state Tracker
typedef struct SimulatorStateStruct {

  EvoLink::InputStatus input_status;
  bool alarm_on;
  bool horn_on;
  bool random_was_seeded;
  bool generate_random_msgs;
  bool monitor_inputs;
  uint32_t last_req_time;

  uint32_t resptime_tach;
  uint32_t resptime_temp;
  uint32_t resptime_vss;
  uint32_t last_random_msg_time;
  uint32_t time_until_wakeup_required;



  SimulatorStateStruct() : input_status(0), alarm_on(false),
    horn_on(false),
    random_was_seeded(false),
    generate_random_msgs(true),
    monitor_inputs(true),
    last_req_time(0),
    resptime_tach(0),
    resptime_temp(0),
    resptime_vss(0),
    last_random_msg_time(0),
    time_until_wakeup_required(time_until_wakeup_required_default)
  {

  }


} SimulatorState ;

SimulatorState CurrentState;

// forward declerations
void emit(uint8_t code);

void sendRandomEvent(bool doForce);
void toggleDoor();
void toggleHood();
void toggleBrake();
void pollInputs();
void handleRequest(uint8_t reqCode, uint32_t timeNow);
bool hasPendingRequiredResponses();

/*
 * SerialUI callbacks
 */
void cbSetWakeupTimeout()
{
  mySUI.print(F("Current wakeup timeout is "));
  mySUI.print(CurrentState.time_until_wakeup_required);
  mySUI.println(F(" ms."));


  mySUI.print(F("Enter max time between reqs for wakeup (ms): "));

  mySUI.showEnterNumericDataPrompt();
  unsigned long val = mySUI.parseULong();
  mySUI.println(' ');

  if (val < 1 || val > 120000)
    return mySUI.returnError("Invalid value");

  CurrentState.time_until_wakeup_required = val;

  return mySUI.returnOK();

}

void cbRandom()
{
  CurrentState.generate_random_msgs = ! CurrentState.generate_random_msgs;

  mySUI.print(F("Random event gen: "));
  if (CurrentState.generate_random_msgs)
    mySUI.println(F("ON"));
  else
    mySUI.println(F("OFF"));

  return mySUI.returnOK();
}


void cbMonitorInputs()
{
  CurrentState.monitor_inputs = ! CurrentState.monitor_inputs;

  mySUI.print(F("Input monitoring: "));
  if (CurrentState.monitor_inputs)
    mySUI.println(F("ON"));
  else
    mySUI.println(F("OFF"));

  return mySUI.returnOK();
}

void cbGenerateRandom()
{
  sendRandomEvent(true);
  return mySUI.returnOK();
}


void cbSendCode()
{

  mySUI.print(F("Enter code to send (hex): "));
  mySUI.showEnterNumericDataPrompt();
  int code = mySUI.parseIntHex();

  mySUI.println(' ');

  if (code > 0xff || code < 0)
    return mySUI.returnError("Invalid value");

  emit(code);

  return mySUI.returnOK();

}

void cbWakeUp()
{

  mySUI.print(F("Waking up!"));
  handleRequest(REQ_WAKEUP, millis());
  return mySUI.returnOK();
}

void cbStatusReq()
{

  mySUI.println(F("Simulating status request."));
  handleRequest(REQ_REQUEST_INPUT, millis());
  return mySUI.returnOK();

}
void cbSimulateRcv()
{

  mySUI.print(F("Enter code to 'receive' (hex): "));
  mySUI.showEnterNumericDataPrompt();
  int code = mySUI.parseIntHex();

  mySUI.println(' ');

  if (code > 0xff || code < 0)
    return mySUI.returnError("Invalid value");

  handleRequest(code, millis());


  return mySUI.returnOK();
}

void cbToggleDoor() {

  toggleDoor();

  return mySUI.returnOK();


}
void cbToggleHood() {

  toggleHood();

  return mySUI.returnOK();


}
void cbToggleBrake() {
  toggleBrake();
}




void displayState() {

  digitalWrite(door_output_pin, (CurrentState.input_status.door == EvoLink::State::Open));
  digitalWrite(alarm_output_pin, CurrentState.alarm_on);
  digitalWrite(horn_output_pin, CurrentState.horn_on);
}

void toggleDoor() {


  mySUI.print(F("Toggle door: "));

  if (CurrentState.input_status.door == State::Open)
  {

    mySUI.println(F("closed"));
    CurrentState.input_status.door = State::Closed;
    emit(MSG_DOOR_CLOSED);
  } else {

    mySUI.println(F("opened"));
    CurrentState.input_status.door = State::Open;
    emit(MSG_DOOR_OPENED);
  }
  displayState();

}


void toggleHood() {

  mySUI.print(F("Toggle hood: "));
  if (CurrentState.input_status.hood == State::Open)
  {
    mySUI.println(F("closed"));
    CurrentState.input_status.hood = State::Closed;
    emit(MSG_HOOD_CLOSED);
  } else {

    mySUI.println(F("opened"));
    CurrentState.input_status.hood = State::Open;
    emit(MSG_HOOD_OPENED);
  }

  displayState();

}

void toggleTrunk() {


  mySUI.print(F("Toggle trunk: "));
  if (CurrentState.input_status.trunk == State::Open)
  {
    mySUI.println(F("closed"));
    CurrentState.input_status.trunk = State::Closed;
    emit(MSG_TRUNK_CLOSED);
  } else {

    mySUI.println(F("opened"));
    CurrentState.input_status.trunk = State::Open;
    emit(MSG_TRUNK_OPENED);
  }

  displayState();

}



void toggleAlarm() {
  CurrentState.alarm_on = !CurrentState.alarm_on;
  displayState();

}


void toggleBrake() {

  CurrentState.input_status.brake =
    (CurrentState.input_status.brake == State::On ) ?
    State::Off : State::On;

  mySUI.print(F("Toggle brake: "));
  if (CurrentState.input_status.brake == State::On)
  {

    mySUI.println(F("on"));
    emit(MSG_BRAKE_ON);
  } else {

    mySUI.println(F("off"));
    emit(MSG_BRAKE_OFF);
  }
  displayState();
}


void flashDoor() {
  digitalWrite(door_output_pin, LOW);
  delay(200);
  digitalWrite(door_output_pin, HIGH);
  delay(200);
  digitalWrite(door_output_pin, LOW);
  delay(200);
}

void flashHorn(uint16_t dtime)
{

  digitalWrite(horn_output_pin, LOW);
  delay(50);
#ifdef OUTPUT_DEBUG_INFO
  mySUI.print(F("Flashing horn for "));
  mySUI.print(dtime);
  mySUI.println(F(" ms."));
#endif
  digitalWrite(horn_output_pin, HIGH);
  delay(dtime);
  digitalWrite(horn_output_pin, LOW);
}


void emit(uint8_t code)
{
  mySUI.print(F("SEND: 0x"));
  mySUI.println((int)code, HEX);
  (EVO_SERIAL_DEVICE_PTR)->write((int)code);

}

#define RETURN_RNDEVENT_INVALID(doFindValid)  { \
    if (doFindValid) \
      return sendRandomEvent(true); \
    return; \
  }


void sendRandomEvent(bool doForce = false)
{

  DataLink::MessageCode possibleCodes[] = {

#ifdef OUTPUT_RANDOM_EVENTS_REMSTART
    DataLink::RemoteStarter_Disarm,
    DataLink::RemoteStarter_Arm,
    DataLink::RemoteStarter_On,
    DataLink::RemoteStarter_Off,
    DataLink::RemoteStarter_UnlockDisarm,
    DataLink::RemoteStarter_LockArm,
#endif

#ifdef OUTPUT_RANDOM_EVENTS_OPENCLOSE
    DataLink::Door_Opened,
    DataLink::Door_Closed,
    DataLink::Hood_Opened,
    DataLink::Hood_Closed,
    DataLink::Trunk_Opened,
    DataLink::Trunk_Closed,
#endif


#ifdef OUTPUT_RANDOM_EVENTS_BRAKES
    DataLink::HandBrake_On,
    DataLink::HandBrake_Off,
    DataLink::Brake_On,
    DataLink::Brake_Off,
#endif


#ifdef OUTPUT_RANDOM_EVENTS_SENSOR
    DataLink::VSS_Over_15MPH,
    DataLink::ShockSensor_Trigger,
    DataLink::AlarmSensor_PreWarn,
    DataLink::TiltSensor_Trigger,
#endif

#ifdef OUTPUT_RANDOM_EVENTS_TACH
    DataLink::Tach_On,
    DataLink::Tach_Off,
    DataLink::Tach_OverRev,
#endif


#ifdef OUTPUT_RANDOM_EVENTS_MISC
    DataLink::CarKey_In_On,
    DataLink::CarKey_In_Off,
    DataLink::RemoteProgramming_Enable,
    DataLink::RemoteProgramming_Disable,
#endif

    DataLink::Temperature_Error // just so we are guaranteed to have one entry

  };

  uint8_t numElements = ( sizeof(possibleCodes) / sizeof(DataLink::MessageCode));

  DataLink::MessageCode selectedCode = possibleCodes[random() % numElements];

  if (selectedCode == DataLink::Temperature_Error)
    return;

  // we want to keep track of a few of these...


  switch (selectedCode) {
    case DataLink::Door_Opened:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.door == State::Open)
        RETURN_RNDEVENT_INVALID(doForce);


#endif

      CurrentState.input_status.door = State::Open;
      mySUI.println(F("gen: door open"));

      break;

    case DataLink::Door_Closed:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.door == State::Closed)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.door = State::Closed;
      mySUI.println(F("gen: door close"));
      break;
    case DataLink::Hood_Opened:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.hood == State::Open)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.hood = State::Open;
      mySUI.println(F("gen: hood open"));
      break;

    case DataLink::Hood_Closed:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.hood == State::Closed)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.hood = State::Closed;
      mySUI.println(F("gen: hood close"));
      break;
    case DataLink::Trunk_Opened:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.trunk == State::Open)
        RETURN_RNDEVENT_INVALID(doForce);
#endif

      CurrentState.input_status.trunk = State::Open;
      mySUI.println(F("gen: trunk open"));
      break;
    case DataLink::Trunk_Closed:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.trunk == State::Closed)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.trunk = State::Closed;
      mySUI.println(F("gen: trunk close"));
      break;

    case DataLink::HandBrake_On:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.hand_brake == State::On)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.hand_brake = State::On;
      mySUI.println(F("gen: handbrake on"));
      break;
    case DataLink::HandBrake_Off:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.hand_brake == State::Off)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.hand_brake = State::Off;
      mySUI.println(F("gen: handbrake off"));
      break;

    case DataLink::Brake_On:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.brake == State::On)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.brake = State::On;
      mySUI.println(F("gen: brake on"));
      break;
    case DataLink::Brake_Off:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.brake == State::Off)
        RETURN_RNDEVENT_INVALID(doForce);

#endif
      CurrentState.input_status.brake = State::Off;
      mySUI.println(F("gen: brake off"));
      break;

    case DataLink::Tach_On:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.tach == State::On)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.tach = State::On;
      mySUI.println(F("gen: tach on"));
      break;
    case DataLink::Tach_Off:
#ifdef CONSISTENT_STATE_RANDOM_EVENTS
      if (CurrentState.input_status.tach == State::Off)
        RETURN_RNDEVENT_INVALID(doForce);
#endif
      CurrentState.input_status.tach = State::Off;
      mySUI.println(F("gen: tach off"));
      break;


    case DataLink::RemoteStarter_Disarm:
      /* fall-through */
    case DataLink::RemoteStarter_Arm:
      /* fall-through */
    case DataLink::RemoteStarter_On:
      /* fall-through */
    case DataLink::RemoteStarter_Off:
      /* fall-through */
    case DataLink::RemoteStarter_UnlockDisarm:
      /* fall-through */
    case DataLink::RemoteStarter_LockArm:
      mySUI.println(F("gen: rem stater evt"));
      break;


    case DataLink::VSS_Over_15MPH:
      /* fall-through */
    case DataLink::ShockSensor_Trigger:
      /* fall-through */
    case DataLink::AlarmSensor_PreWarn:
      /* fall-through */
    case DataLink::TiltSensor_Trigger:
      mySUI.println(F("gen: sensor evt"));
      break;


    default:
      // do nothing special

      mySUI.println(F("gen: misc"));
      break;
  }

  emit((uint8_t)selectedCode);


}


// requests



void handleRequest(uint8_t reqCode, uint32_t timeNow)
{

#ifdef RANDOMIZE_MSGOUTPUT_BASED_ON_REQ
  if (! CurrentState.random_was_seeded)
  {
    // seed based on time of first request--
    randomSeed(millis());
    CurrentState.random_was_seeded = true;
  }
#endif


  mySUI.print(F("RCVD: "));
  mySUI.println((int)reqCode, HEX);

  if (reqCode == REQ_WAKEUP)
  {
    // just a wakeup call...
#ifdef OUTPUT_DEBUG_INFO
    mySUI.println(F("\nAwoken\n"));
#endif
    CurrentState.last_req_time = timeNow;
    return;
  }

  uint32_t delayBetweenReqs = timeNow - CurrentState.last_req_time;
  if (delayBetweenReqs > CurrentState.time_until_wakeup_required)
  {
    // you need to wake me up!
#ifdef OUTPUT_DEBUG_INFO
    mySUI.print(delayBetweenReqs);
    mySUI.print(F(" > "));
    mySUI.println(CurrentState.time_until_wakeup_required);
#endif
    mySUI.returnError("Wake me up first!");

    return;
  }

  if (delayBetweenReqs < min_delay_between_requests_ms)
  {

    // you need to wake me up!
    delay(min_delay_between_requests_ms - delayBetweenReqs);
  }

  // ok, timing is good...
  CurrentState.last_req_time = timeNow;


  uint16_t hornOnTime = 0;
  switch (reqCode)
  {
    case REQ_REQUEST_INPUT:
      // request input
#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Current status requested..."));
      mySUI.print(F("\tBrake:\t"));
      mySUI.println( (CurrentState.input_status.brake == State::On) ? F("ON") : F("OFF"));
      mySUI.print(F("\tHBrake:\t"));
      mySUI.println( (CurrentState.input_status.hand_brake == State::On) ? F("ON") : F("OFF"));
      mySUI.print(F("\tDoor:\t"));
      mySUI.println( (CurrentState.input_status.door == State::Open) ? F("Open") : F("Closed"));
      mySUI.print(F("\tTrunk:\t"));
      mySUI.println( (CurrentState.input_status.trunk == State::Open) ? F("Open") : F("Closed"));
      mySUI.print(F("\tHood:\t"));
      mySUI.println( (CurrentState.input_status.hood == State::Open) ? F("Open") : F("Closed"));
      mySUI.print(F("\tTach:\t"));
      mySUI.println( (CurrentState.input_status.tach == State::On) ? F("ON") : F("OFF"));
      mySUI.println( '\n');

#endif
      emit(CurrentState.input_status.asByte());
      break;

    case REQ_PING:
      // respond

#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Responding to ping"));
#endif
      emit(MSG_PING);
      break;
    case REQ_PONG:
      // respond
#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Responding to pong"));
#endif
      emit(MSG_PONG);
      break;
    case REQ_DRIVER1_LOCK:
      /* fall through */
    case REQ_DRIVER1_UNLOCK:
      /* fall through */
    case REQ_DRIVER2_LOCK:
      /* fall through */
    case REQ_DRIVER2_UNLOCK:
      /* fall through */

#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Door lock/unlock"));
#endif
      flashDoor();
      break;

    case REQ_ALARM_ON:
#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Alarm Activated"));
#endif
      CurrentState.alarm_on = true;
      break;

    case REQ_ALARM_OFF:
#ifdef OUTPUT_DEBUG_INFO
      mySUI.println(F("Alarm Deactivated"));
#endif
      CurrentState.alarm_on = false;
      break;

    case REQ_HORN_ON:
      CurrentState.horn_on = true;
      break;

    case REQ_HORN_OFF:
      CurrentState.horn_on = false;
      break;

    case REQ_HORN_ON_250MS:
      hornOnTime = 250;
      break;
    case REQ_HORN_ON_50MS:
      hornOnTime = 50;
      break;
    case REQ_HORN_ON_15MS:
      hornOnTime = 15;
      break;

    case REQ_REQUEST_TACH:
      CurrentState.resptime_tach = timeNow + datarequest_response_delay_ms;
      mySUI.print(F("Tach resp scheduled for "));
      mySUI.println(CurrentState.resptime_tach);
      break;

    case REQ_REQUEST_VSS:
      CurrentState.resptime_vss = timeNow + datarequest_response_delay_ms;
      mySUI.print(F("VSS resp scheduled for "));
      mySUI.println(CurrentState.resptime_vss);
      break;

    case REQ_REQUEST_TEMPERATURE:
      CurrentState.resptime_temp = timeNow + datarequest_response_delay_ms;
      mySUI.print(F("Temp resp scheduled for "));
      mySUI.println(CurrentState.resptime_temp);
      break;

    default:
      // don't care...
      break;
  }

  if (hornOnTime)
  {
    flashHorn(hornOnTime * horn_on_time_multiplier);

  }



  displayState(); // make sure state is consistent
}

#define max_requests_per_loop	20
void parsePendingRequests(uint32_t timeNow)
{
  if (! (EVO_SERIAL_DEVICE_PTR)->available())
    return; // nothing to do...

  uint8_t num_reqs = 0;
  do {
    uint8_t aByte = (uint8_t)(EVO_SERIAL_DEVICE_PTR)->read();
    handleRequest(aByte, timeNow);


  } while ((EVO_SERIAL_DEVICE_PTR)->available() && (num_reqs++ < max_requests_per_loop) );
}

bool hasPendingRequiredResponses()
{
  return (CurrentState.resptime_tach || CurrentState.resptime_vss || CurrentState.resptime_temp);

}

void returnRequiredResponses(uint32_t timeNow)
{

  if (CurrentState.resptime_tach && (timeNow >= CurrentState.resptime_tach))
  {
    mySUI.println(F("Tach resp."));
    CurrentState.resptime_tach = 0; // reset.

    // send appropriate answer now
    emit((random() % 0x6c));


    return;
  }


  if (CurrentState.resptime_vss && (timeNow >= CurrentState.resptime_vss))
  {

    mySUI.println(F("VSS resp."));
    CurrentState.resptime_vss = 0; // reset.
    // send appropriate answer now
    emit((random() % 200));


    return;
  }



  if (CurrentState.resptime_temp && (timeNow >= CurrentState.resptime_temp))
  {

    int theTemp = (random() % 100);

    mySUI.print(F("Temp resp: "));
    mySUI.println(theTemp - 20);

    CurrentState.resptime_temp = 0; // reset.

    // send appropriate answer now
    emit(168 + theTemp - 20); // goes from -20 to 80


    return;
  }

}



void pollInputs()
{
  if (! CurrentState.monitor_inputs)
    return;

  if (digitalRead(brake_input_pin))
  {
    if (CurrentState.input_status.brake != State::On)
    {
      // first application of brakes
      CurrentState.input_status.brake = State::On;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Brake_On);
    }
  } else {
    if (CurrentState.input_status.brake == State::On)
    {
      // break released
      CurrentState.input_status.brake = State::Off;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Brake_Off);
    }
  }




  if (digitalRead(door_input_pin))
  {
    if (CurrentState.input_status.door != State::Open)
    {
      // door opened
      CurrentState.input_status.door = State::Open;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Door_Opened);
    }
  } else {
    if (CurrentState.input_status.door == State::Open)
    {
      // break released
      CurrentState.input_status.door = State::Closed;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Door_Closed);
    }
  }


  if (digitalRead(hood_input_pin))
  {
    if (CurrentState.input_status.hood != State::Open)
    {
      // door opened
      CurrentState.input_status.hood = State::Open;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Hood_Opened);
    }
  } else {
    if (CurrentState.input_status.hood == State::Open)
    {
      // break released
      CurrentState.input_status.hood = State::Closed;
      mySUI.println_P(msg_input_action);
      emit((uint8_t)DataLink::Hood_Closed);
    }
  }



}

void simulatorLoop()
{

  uint32_t timeNow = millis();

  pollInputs();

  // get anything waiting for processing
  parsePendingRequests(timeNow);

  timeNow = millis();
  // return any required data who's time has come.
  returnRequiredResponses(timeNow);

  // generate some random noise, if it's time
  if (CurrentState.generate_random_msgs &&
      (timeNow >= (CurrentState.last_random_msg_time + random_msg_interval_ms)))
  {
    CurrentState.last_random_msg_time = timeNow;
    sendRandomEvent();
  }


  returnRequiredResponses(millis());

}

void setupMenus() {
  // Get a handle to the top level menu
  // Note that menus are returned as pointers.
  SUI::Menu * mainMenu = mySUI.topLevelMenu();
  if (!mainMenu) {
    // what? Could not create :(
    return mySUI.returnError(
             "Something is very wrong, could not get top level menu?");

  }

  mainMenu->setName(top_menu_title);

  if (!mainMenu->addCommand(door_toggle, cbToggleDoor, door_toggle_help)) {
    return mySUI.returnError_P(error_cant_add);
  }
  if (!mainMenu->addCommand(hood_toggle, cbToggleHood, hood_toggle_help)) {
    return mySUI.returnError_P(error_cant_add);
  }
  if (!mainMenu->addCommand(brake_toggle, cbToggleBrake, brake_toggle_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(send_code, cbSendCode, send_code_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(sim_rcv, cbSimulateRcv, sim_rcv_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(status_req, cbStatusReq, status_req_help)) {
    return mySUI.returnError_P(error_cant_add);
  }
  if (!mainMenu->addCommand(wake_up_cmd, cbWakeUp, wake_up_cmd_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(gen_rnd_events, cbGenerateRandom,
                            gen_rnd_events_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(toggle_input_mon, cbMonitorInputs,
                            toggle_input_mon_help)) {
    return mySUI.returnError_P(error_cant_add);
  }

  if (!mainMenu->addCommand(toggle_rnd_events, cbRandom,
                            toggle_rnd_events_help)) {
    return mySUI.returnError_P(error_cant_add);
  }
  if (!mainMenu->addCommand(wakeup_timeout, cbSetWakeupTimeout,
                            wakeup_timeout_help)) {
    return mySUI.returnError_P(error_cant_add);
  }
  // Done setting up the menus!

}


/*
 * Standard setup().
 *
 * We'll setup the debug serial output, tell EvoLink about the
 * callbacks we made and call EVO.begin() to get going.
 */
void setup()
{


#ifdef USING_PLATFORM_XMEGA

  // Enable 32M internal crystal
  OSC.CTRL |= OSC_RC32MEN_bm;
  // Wait for it to stablize
  while ( !(OSC.STATUS & OSC_RC32MEN_bm) ) ;

  // Set main system clock to 32Mhz internal clock
  CCP = CCP_IOREG_gc; // Secret handshake so we can change clock
  CLK.CTRL = (CLK.CTRL & ~CLK_SCLKSEL_gm ) | CLK_SCLKSEL_RC32M_gc;
#endif


  // startup our serial ui USART
  DEBUG_SERIAL.begin(DEBUG_BAUD_RATE);
  EVO_SERIAL_DEVICE_PTR->begin(EVO_BAUD_RATE);

  mySUI.setTimeout(20000);
  // setup our i/o
  pinMode(brake_input_pin, INPUT);
  pinMode(door_input_pin, INPUT);
  pinMode(hood_input_pin, INPUT);

  pinMode(horn_output_pin, OUTPUT);
  pinMode(door_output_pin, OUTPUT);
  pinMode(alarm_output_pin, OUTPUT);


  // align our LEDs with "reality"
  displayState();


  setupMenus();

  DEBUG_SERIAL.println(F("Setup complete--will now generate events/respond to requests..."));


}


void loop()
{

  // always run the simulator
  simulatorLoop();

  if (mySUI.checkForUser(150))
  {
    // we have a user initiating contact, show the
    // greeting message and prompt
    mySUI.enter();


    /* Now we keep handling the serial user's
     ** requests until they exit or timeout.
     */
    while (mySUI.userPresent())
    {
      // actually respond to requests, using
      mySUI.handleRequests();
      simulatorLoop(); // keep the simulation going...
    }

  } /* end if we had a user on the serial line */

}