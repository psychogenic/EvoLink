/*
 * driver.cpp -- Driver implementation for EvoLink,
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
 */

#include "includes/driver.h"
#include "includes/platform.h"

/*
 * Event dispatchers
 *
 * These dispatchers are used internally to trigger appropriate callbacks
 * for various event (message) codes received.
 *
 */
void dispatch_event_remstart(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.remotestarter_event)
		EVO.callbacks.remotestarter_event((EvoLink::RemoteStarter::Event)message);

}

void dispatch_event_openclose(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.openclose_event)
		EVO.callbacks.openclose_event((EvoLink::OpenClose::Event)message);

}

void dispatch_event_brake(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.brake_event)
		EVO.callbacks.brake_event((EvoLink::Brake::Event)message);

}
void dispatch_event_tach(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.tach_event)
		EVO.callbacks.tach_event((EvoLink::Tach::Event)message);

}

void dispatch_event_sensor(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.sensor_event)
		EVO.callbacks.sensor_event((EvoLink::Sensor::Event)message);

}

void dispatch_generic_message(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.message_received)
		EVO.callbacks.message_received(message);

}

void dispatch_error_message(EvoLink::DataLink::MessageCode message)
{
	if (EVO.callbacks.error_event)
		EVO.callbacks.error_event((EvoLink::ErrorMessage::Event)message, 0);

}


int request_response_temperature(EvoLink::DataLink::RequestCode request, uint8_t returnedValue)
{
	int realVal = returnedValue;
	realVal -= 168;

	return realVal;
}


int request_response_tach(EvoLink::DataLink::RequestCode request, uint8_t returnedValue)
{
	int realVal = (returnedValue << 8);

	return realVal;
}


void synchronous_getter_handler(EvoLink::DataLink::RequestCode request,
		int value)
{
	// store the received value
	EVO.synch_getter_value_received = value;
}

EvoLink::EvoAll EVO;

namespace EvoLink {

/*
 * Callback association table for supported incoming message types.
 */
EvoAll::MessageCodeCallback EvoAll::callback_assocs[] = {

		// brake events
		EvoAll::MessageCodeCallback(DataLink::Brake_On, dispatch_event_brake),
		EvoAll::MessageCodeCallback(DataLink::Brake_Off, dispatch_event_brake),
		EvoAll::MessageCodeCallback(DataLink::HandBrake_On, dispatch_event_brake),
		EvoAll::MessageCodeCallback(DataLink::HandBrake_Off, dispatch_event_brake),

		// tach events
		EvoAll::MessageCodeCallback(DataLink::Tach_On, dispatch_event_tach),
		EvoAll::MessageCodeCallback(DataLink::Tach_Off, dispatch_event_tach),
		EvoAll::MessageCodeCallback(DataLink::Tach_OverRev, dispatch_event_tach),

		// remote starter events
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_Disarm, dispatch_event_remstart),
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_Arm, dispatch_event_remstart),
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_On, dispatch_event_remstart),
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_Off, dispatch_event_remstart),
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_UnlockDisarm, dispatch_event_remstart),
		EvoAll::MessageCodeCallback(DataLink::RemoteStarter_LockArm, dispatch_event_remstart),

		// open-close events
		EvoAll::MessageCodeCallback(DataLink::Door_Opened, dispatch_event_openclose),
		EvoAll::MessageCodeCallback(DataLink::Door_Closed, dispatch_event_openclose),
		EvoAll::MessageCodeCallback(DataLink::Hood_Opened, dispatch_event_openclose),
		EvoAll::MessageCodeCallback(DataLink::Hood_Closed, dispatch_event_openclose),
		EvoAll::MessageCodeCallback(DataLink::Trunk_Opened, dispatch_event_openclose),
		EvoAll::MessageCodeCallback(DataLink::Trunk_Closed, dispatch_event_openclose),

		// sensor events
		EvoAll::MessageCodeCallback(DataLink::ShockSensor_Trigger, dispatch_event_sensor),
		EvoAll::MessageCodeCallback(DataLink::AlarmSensor_PreWarn, dispatch_event_sensor),
		EvoAll::MessageCodeCallback(DataLink::TiltSensor_Trigger, dispatch_event_sensor),
		EvoAll::MessageCodeCallback(DataLink::VSS_Over_15MPH, dispatch_event_sensor),


		// generic events
		EvoAll::MessageCodeCallback(DataLink::Ping_Message, dispatch_generic_message),
		EvoAll::MessageCodeCallback(DataLink::Pong_Message, dispatch_generic_message),
		EvoAll::MessageCodeCallback(DataLink::CarKey_In_On, dispatch_generic_message),
		EvoAll::MessageCodeCallback(DataLink::CarKey_In_Off, dispatch_generic_message),
		EvoAll::MessageCodeCallback(DataLink::RemoteProgramming_Enable, dispatch_generic_message),
		EvoAll::MessageCodeCallback(DataLink::RemoteProgramming_Disable, dispatch_generic_message),

		// error events

		// WARNING: code below expects DataLink::Temperature_Error to be the
		// LAST VALUE in this list--leave as array terminator, or adapt code as required
		EvoAll::MessageCodeCallback(DataLink::Temperature_Error, dispatch_error_message),

};


EvoAll::RequestWithResponse EvoAll::reqs_with_responses[] = {
		EvoAll::RequestWithResponse(DataLink::Request_VSS),
		EvoAll::RequestWithResponse(DataLink::Request_Tach, request_response_tach),
		EvoAll::RequestWithResponse(DataLink::Request_Input),

		// WARNING: code below expects Request_Temperature to be the
		// LAST VALUE in this list--leave as array terminator, or adapt code as required
		EvoAll::RequestWithResponse(DataLink::Request_Temperature, request_response_temperature)

};


EvoAll::EvoAll() :
		callbacks(),
		synch_getter_value_received(-1),
		serial_setup(NULL),
		pending_request_response(NULL),
		pending_request_issue_time(0),
#ifdef MINTIME_BETWEEN_WAKEUPS_MS
		last_wakeup_time(0),
#endif
		last_tx_time(0),
		auto_delay_ms(AUTODELAY_DEFAULT_MS),
		user_specified_handler(NULL)
{

}
void EvoAll::begin(SerialSetup serial)
{
	serial_setup = serial;
	SerialConnection::setup(serial);

}


void EvoAll::delayWhileCheckingActivity(uint16_t delayMillis, uint16_t activityCheckPeriod)
{

	uint32_t timeNow = timeMs();
	uint32_t nextCheckTime = (timeMs() + (uint32_t)activityCheckPeriod);
	uint32_t timeoutTime = (timeNow + (uint32_t)delayMillis);

	checkActivity(); // do a preliminary  checkActivity() so we have at least 1
	while (timeNow < timeoutTime)
	{
		if (timeNow >= nextCheckTime)
		{
			checkActivity();
			nextCheckTime = timeNow + (uint32_t)activityCheckPeriod;
		}

		delayMs(20);

		timeNow = timeMs();

	}

	return;

}
void EvoAll::checkActivity(uint16_t timeout)
{
	bool msgRcvd = false;
	uint32_t maxMs = (timeMs() + (uint32_t)timeout);


	int c;
	do {
		while (SerialConnection::available())
		{
			c = SerialConnection::read();
#ifdef DEBUG_USART_ENABLE
			if (serial_setup.debug_usart)
			{
				serial_setup.debug_usart->print(F("Evo rcvd: 0x"));
				serial_setup.debug_usart->println(c, HEX);
			}
#endif


			msgRcvd = parseMessage(c);
			if (! SerialConnection::available())
			{
				// apparently nothing left in the buffer
				// allow a little time to get the next byte,
				// if it's actually still coming down the wire.
				// Assuming 9600 baud, 8+2 bits would take a bit less than 1ms
				delayUs(950);
			}

		}

		if (timeout)
		{
			delayUs(500);
		}


	} while ((!msgRcvd) && (maxMs > timeMs()));
}


EvoAll::MessageCodeCallback * EvoAll::codeToCallbackEntryFor(uint8_t raw_msg_code)
{

	uint8_t termination_entry = (uint8_t)(DataLink::Temperature_Error);


	uint8_t i=0;
	do {

		if (callback_assocs[i].raw_msg_code == raw_msg_code)
		{
			// found it...
			return &(callback_assocs[i]);

		}

	} while (callback_assocs[i++].raw_msg_code != termination_entry);


	// if we get here, we don't know whatch you're talkin' 'bout, Willis...
	return NULL;
}

GenericMessageHandler EvoAll::handlerForMessage(DataLink::MessageCode msg)
{
	return handlerForMessage((uint8_t)msg);
}


GenericMessageHandler EvoAll::handlerForMessage(uint8_t raw_msg_code)
{

	EvoAll::MessageCodeCallback * entryForMsg = codeToCallbackEntryFor(raw_msg_code);

	if (entryForMsg) {
		return entryForMsg->callback;
	}

	// not found
	return NULL;


}
bool EvoAll::setHandlerForMessage(DataLink::MessageCode code, GenericMessageHandler useCallback)
{

	EvoAll::MessageCodeCallback * entryForMsg = codeToCallbackEntryFor((uint8_t)code);
	if (entryForMsg) {
		entryForMsg->callback = useCallback;
		return true;
	}

	// couldn't locate, couldn't set.
	return false;

}


bool EvoAll::parseMessage(int msg)
{
	if (msg < 0)
		return false; // nothing read

	// first, check if we're currently waiting on a response byte to one of the data requests
	if (pending_request_response)
	{
		// yep!

		// first, make sure it's even possible that this is our message (if it arrives too
		// quickly, might be a race condition--i.e. send tach request, door opens, tach req response arrives
		uint32_t curTime = timeMs();

		if (curTime < pending_request_issue_time)
		{


#ifdef DEBUG_USART_ENABLE
			if (serial_setup.debug_usart)
			{
				serial_setup.debug_usart->println(F("Evo expt resp time loop"));
			}
#endif

			// edge case -- we've looped around the system timer just as we were awaiting a response
			if (callbacks.error_event)
				callbacks.error_event(ErrorMessage::RequestTimeout, pending_request_response->req);

			resetPendingRequest(); // scrap it.
			return false;
		} else if (curTime >= (pending_request_issue_time + ABS_REQUEST_RESPONSE_TIME_MINIMUM_MS))
		{


			// this should be the response we were hoping for


#ifdef DEBUG_USART_ENABLE
			if (serial_setup.debug_usart)
			{
				serial_setup.debug_usart->println(F("Evo got resp"));
			}
#endif



			if (callbacks.requested_data_received)
			{
				int respVal = (pending_request_response->processor != NULL) ?
						pending_request_response->processor(pending_request_response->req, msg) : msg;

#ifdef DEBUG_USART_ENABLE
				if (serial_setup.debug_usart)
				{
					serial_setup.debug_usart->print(F("Evo calling handle w/val: "));
					serial_setup.debug_usart->println(respVal, DEC);
				}
#endif



				callbacks.requested_data_received(pending_request_response->req, respVal);
			}


			resetPendingRequest();
			return true;
		}


	}


	// we get here, either we aren't waiting for a response, or it's still too early for it
	// to arrive--must be a standard message...

	uint8_t msgcode = (uint8_t)msg;
	GenericMessageHandler handler = handlerForMessage(msgcode);

	if (handler) {
		// got a handler for this message type -- use it.
		handler((DataLink::MessageCode) msgcode);
		return true;
	}

	// could not locate...
	if (callbacks.error_event) {
		// have an error handler setup, notify:
		callbacks.error_event(ErrorMessage::UnsupportedValue, msgcode);
	}


	return false;

}


bool EvoAll::wakeUp()
{
	// handle the wake-up a bit differently, as we
	// a) know it doesn't get a response and
	// b) may want to do wakeups automatically

#ifdef MINTIME_BETWEEN_WAKEUPS_MS
	uint32_t timenow = timeMs();
	if ( (timenow - last_wakeup_time) < MINTIME_BETWEEN_WAKEUPS_MS)
	{
		// ignore this, too soon...
#ifdef DEBUG_USART_ENABLE
			if (serial_setup.debug_usart)
			{
				serial_setup.debug_usart->println(F("Evo::wakeUp() but already awake, skip."));
			}
#endif

		return false;
	}
	// ok, been long enough to issue a new wake-up
	last_wakeup_time = timenow;
#endif

	sendRequest(DataLink::WakeUp);

#ifdef POSTWAKEUP_AUTO_DELAY_MS
	delayMs(POSTWAKEUP_AUTO_DELAY_MS);
#endif
	return true;
}







/* request shortcut methods
 * Most of these offer two methods of access:
 * xxxOn()/xxxOff()
 * xxx(State::SetTo) -- so xxx(State::On) and xxx(State::Off)
 *
 */
bool EvoAll::systemArm() {
	return makeRequest(DataLink::System_Arm);
}
bool EvoAll::systemDisarm() {
	return makeRequest(DataLink::System_Disarm);
}
bool EvoAll::arm(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ?
					DataLink::System_Arm : DataLink::System_Disarm);
}

bool EvoAll::groundOutOn() {
	bool r = makeRequest(DataLink::GroundOut_On);
#ifdef POSTGROUNDOUT_ON_DELAY_MS
	delayMs(POSTGROUNDOUT_ON_DELAY_MS);
#endif
	return r;
}
bool EvoAll::groundOutOff() {
	return makeRequest(DataLink::GroundOut_Off);
}
bool EvoAll::groundOut(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ?
					DataLink::GroundOut_On : DataLink::GroundOut_Off);
}

bool EvoAll::trunkRelease() {
	return makeRequest(DataLink::Trunk_Release);
}

bool EvoAll::panicOn() {
	return makeRequest(DataLink::Panic_On);
}
bool EvoAll::panicOff() {
	return makeRequest(DataLink::Panic_Off);
}
bool EvoAll::panic(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ? DataLink::Panic_On : DataLink::Panic_Off);
}

bool EvoAll::parklightOn() {
	return makeRequest(DataLink::ParkingLight_On);
}
bool EvoAll::parklightOff() {
	return makeRequest(DataLink::ParkingLight_Off);
}
bool EvoAll::parklight(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ?
					DataLink::ParkingLight_On : DataLink::ParkingLight_Off);
}

bool EvoAll::alarmOn() {
	return makeRequest(DataLink::Alarm_On);
}
bool EvoAll::alarmOff() {
	return makeRequest(DataLink::Alarm_Off);
}
bool EvoAll::alarm(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ? DataLink::Alarm_On : DataLink::Alarm_Off);
}

bool EvoAll::accessoryOn() {
	return makeRequest(DataLink::Accessory_On);
}
bool EvoAll::accessoryOff() {
	return makeRequest(DataLink::Accessory_Off);
}
bool EvoAll::accessory(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ?
					DataLink::Accessory_On : DataLink::Accessory_Off);
}

bool EvoAll::starterKillOn() {
	return makeRequest(DataLink::StarterKill_On);
}
bool EvoAll::starterKillOff() {
	return makeRequest(DataLink::StarterKill_Off);
}
bool EvoAll::starterKill(State::SetTo onOrOff) {
	return makeRequest(
			(onOrOff == State::On) ?
					DataLink::StarterKill_On : DataLink::StarterKill_Off);
}

// lock/unlock -- call specifying Driver::One or Driver::Two
bool EvoAll::lock(Driver::Selection d) {
	return makeRequest(
			(d == Driver::One) ?
					DataLink::Driver1_Lock : DataLink::Driver2_Lock);
}
bool EvoAll::unlock(Driver::Selection d) {
	return makeRequest(
			(d == Driver::One) ?
					DataLink::Driver1_Unlock : DataLink::Driver2_Unlock);
}
bool EvoAll::priorityUnlock(Driver::Selection d) {
	return makeRequest(
			(d == Driver::One) ?
					DataLink::Driver1_Priority_Unlock :
					DataLink::Driver2_Priority_Unlock);
}

/*
 * Methods offering Source options (from remote or from key), so are called with
 * xxxOn(Source::Key) or xxxOn(Source::Remote)
 *
 */
bool EvoAll::ignitionOn(Source::TriggerSource src) {
	return makeRequest(
			(src == Source::Key) ?
					DataLink::Ignition_FromKey_On :
					DataLink::Ignition_FromRemote_On);
}
bool EvoAll::ignitionOff(Source::TriggerSource src) {
	return makeRequest(
			(src == Source::Key) ?
					DataLink::Ignition_FromKey_Off :
					DataLink::Ignition_FromRemote_Off);
}

bool EvoAll::starterOn(Source::TriggerSource src) {
	return makeRequest(
			(src == Source::Key) ?
					DataLink::Starter_FromKey_On :
					DataLink::Starter_FromRemote_On);
}
bool EvoAll::starterOff(Source::TriggerSource src) {
	return makeRequest(
			(src == Source::Key) ?
					DataLink::Starter_FromKey_Off :
					DataLink::Starter_FromRemote_Off);
}

// ping/pong
bool EvoAll::ping() {
	return makeRequest(DataLink::Ping_Request);
}
bool EvoAll::pong() {
	return makeRequest(DataLink::Pong_Request);
}

/* remote start/stop */
bool EvoAll::remoteStart() {
	return makeRequest(DataLink::Remote_Start);
}
bool EvoAll::remoteStop() {
	return makeRequest(DataLink::Remote_Stop);
}
bool EvoAll::remoteToggle() {
	return makeRequest(DataLink::Remote_Toggle);
}

// the request simply performs the request -- it is asynchronous.
// Your callbacks.requested_data_received callback
// function will be called whenever the response comes in.
bool EvoAll::requestTach() {
	return makeRequest(DataLink::Request_Tach);
}
bool EvoAll::requestVSS() {
	return makeRequest(DataLink::Request_VSS);
}
bool EvoAll::requestStatus() {
	return makeRequest(DataLink::Request_Input);
}
bool EvoAll::requestTemperature() {
	return makeRequest(DataLink::Request_Temperature);
}

/*
 * The following getXXX() utility methods are, unlike their corresponding requestXXX() versions
 * above, synchronous -- they will bypass using any callbacks.requested_data_received callback
 * specified and will instead await a response for (up to) timeout ms and either return it
 * or return -1 on failure.
 */
int16_t EvoAll::getTach(uint16_t timeout) {
	return synchronousGet(timeout, DataLink::Request_Tach);
}
int16_t EvoAll::getVSS(uint16_t timeout) {
	return synchronousGet(timeout, DataLink::Request_VSS);
}
int16_t EvoAll::getTemperature(uint16_t timeout) {
	return synchronousGet(timeout, DataLink::Request_Temperature);
}



































bool EvoAll::makeRequest(DataLink::RequestCode reqCode)
{
#ifdef AUTO_CHECKACTIVITY_BEFORE_REQUESTS
	checkActivity();
#endif

	EvoAll::RequestWithResponse * reqsResponse =  EvoAll::reqWithResponseEntryFor(reqCode);

	if (reqsResponse)
	{
		// oh, a special guy...

		if (pending_request_response)
		{
			// one at a time, boys..
			return false;
		}
		prepareRequestNeedingResponse(reqsResponse);
	}

	sendRequest(reqCode);
	return true;
}




InputStatus EvoAll::getStatus(uint16_t timeout) {
	 int16_t val = synchronousGet(timeout, DataLink::Request_Input);
	 if (val < 0)
	 {
		 return InputStatus();
	 }

	 return InputStatus((uint8_t)val);

}

int16_t EvoAll::synchronousGet(uint16_t timeout, DataLink::RequestCode code) {

	checkActivity(105); // clear the buffer


#ifdef DEBUG_USART_ENABLE
	if (serial_setup.debug_usart)
	{
		serial_setup.debug_usart->print(F("Evo synchGet: 0x"));
		serial_setup.debug_usart->println((int)code, HEX);
	}
#endif



	synchronousValueGetterSetup();
	if (makeRequest(code))
	{
		// request sent
		// now get the response
		delay(40);
		checkActivity(timeout);
	}
	synchronousValueGetterTeardown();

	if (synch_getter_value_received < 0)
	{


#ifdef DEBUG_USART_ENABLE
		if (serial_setup.debug_usart)
		{
			serial_setup.debug_usart->println(F("Evo nothing returned in time"));
		}
#endif

		if (callbacks.error_event)
			callbacks.error_event(ErrorMessage::RequestTimeout, code);
	}

	return synch_getter_value_received;

}


void EvoAll::sendRequest(DataLink::RequestCode reqCode)
{
	if (auto_delay_ms)
	{
		uint32_t curTime = timeMs();

		if (curTime < (last_tx_time + (uint32_t)auto_delay_ms))
		{
			
#ifdef DEBUG_USART_ENABLE
			if (serial_setup.debug_usart)
			{

				serial_setup.debug_usart->print(F("Evo autodelaying (ms): "));
				serial_setup.debug_usart->println( ((last_tx_time + auto_delay_ms) - curTime), DEC);
			}
#endif

			delayMs((last_tx_time + (uint32_t)auto_delay_ms) - curTime);
		}

		last_tx_time = timeMs();
	}

#ifdef DEBUG_USART_ENABLE
	if (serial_setup.debug_usart)
	{
		serial_setup.debug_usart->print(last_tx_time, DEC);
		serial_setup.debug_usart->print(F(" Evo send: 0x"));
		serial_setup.debug_usart->println(reqCode, HEX);
	}

#endif
	uint8_t reqCodeV = reqCode;
	SerialConnection::write(reqCodeV);

}


bool EvoAll::horn(Horn::Action act)
{
	DataLink::RequestCode hornReq;
	switch (act)
	{
	case Horn::Off:
		hornReq = DataLink::Horn_Off;
		break;
	case Horn::On:
		hornReq = DataLink::Horn_Off;
		break;

	case Horn::Beep15ms:
		hornReq = DataLink::Horn_On_15ms;
		break;

	case Horn::Beep50ms:
		hornReq = DataLink::Horn_On_50ms;
		break;

	case Horn::Beep250ms:
		hornReq = DataLink::Horn_On_250ms;
		break;

	default:
		return false;
	}

	return makeRequest(hornReq);

}

bool EvoAll::auxiliary(uint8_t index)
{
	DataLink::RequestCode idxToCode[] = {
			DataLink::AUX_1,
			DataLink::AUX_2,
			DataLink::AUX_3,
			DataLink::AUX_4,
			DataLink::AUX_5,
			DataLink::AUX_6,
			DataLink::AUX_7,
			DataLink::AUX_8
	};

	if (index < 1 || index > 8)
		return false;

	return makeRequest(idxToCode[index - 1]);

}
EvoAll::RequestWithResponse * EvoAll::reqWithResponseEntryFor(DataLink::RequestCode code)
{
	uint8_t i=0;
	do {
		if (reqs_with_responses[i].req == code)
		{
			//found it!
			return &(reqs_with_responses[i]);
		}
	} while (reqs_with_responses[i++].req != DataLink::Request_Temperature);

	return NULL;

}
void EvoAll::resetPendingRequest()
{
	pending_request_response = NULL;

}
void EvoAll::prepareRequestNeedingResponse(EvoAll::RequestWithResponse * setup)
{
#ifdef DEBUG_USART_ENABLE
	if (serial_setup.debug_usart)
	{
		serial_setup.debug_usart->print(F("Evo expects response"));
	}
#endif
	pending_request_response = setup;
	pending_request_issue_time = timeMs();

}

void EvoAll::synchronousValueGetterSetup() {

	// backup user-specified handler
	user_specified_handler = callbacks.requested_data_received;

	// setup our own handler
	callbacks.requested_data_received = synchronous_getter_handler;

	// setup our return value, failure as default
	synch_getter_value_received = -1;
}

void EvoAll::synchronousValueGetterTeardown() {
	// restore the user specified handler
	callbacks.requested_data_received = user_specified_handler;
}

} /* namespace EvoLink */
