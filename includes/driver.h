/*
 * driver.h -- Main driver API, part of the
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
 * For general overview, see the EvoLink.h header file, or
 * 	http://flyingcarsandstuff.com/projects/EvoLink
 *
 * The EvoAll driver, exported as a global EVO object, is the
 * instance you use to control the system.
 *
 * You don't need to instantiate it, but you must call begin()
 * with a SerialSetup parameter before using it.
 *
 * In general, you:
 *
 *  - may assign callbacks for various types of EVO-All events,
 *    like remote starter, open/close, brake, sensor or tach events.
 *    See the CallbackSystem and CustomCallbacks examples for
 *    details on these.
 *
 * 	- call begin() and do any setup required
 *
 *  - call various command/request methods to control the vehicle.
 *
 */

#ifndef EVOLINK_DRIVER_H_
#define EVOLINK_DRIVER_H_

#include "serial.h"
#include "types.h"
#include "platform.h"



namespace EvoLink {


class EvoAll {
public:
	EvoAll();

	// Callbacks -- setup at your discretion, to get notified
	// of (categories) of events of interest.
	CallbackContainer callbacks;

	/*
	 * setup/startup
	 */
	void begin(SerialSetup serial);
	uint8_t autoDelayMs() { return auto_delay_ms;}
	void setAutoDelayMs(uint8_t ms) { auto_delay_ms = ms;}

	// You can review/set the handler called for any DataLink::MessageCode.
	// If you override the handler for a particular code by passing a function
	// with signature
	// 					void ... (EvoLink::DataLink::MessageCode message) {}
	// that function will be called when the message arrives,
	// rather than being dispatched to the callback as defined
	// above.
	GenericMessageHandler handlerForMessage(DataLink::MessageCode code);
	GenericMessageHandler handlerForMessage(uint8_t raw_msg_code);
	bool setHandlerForMessage(DataLink::MessageCode code, GenericMessageHandler useCallback);


	/*
	 * check serial conn for incoming messages and
	 * dispatch as appropriate
	 */
	void checkActivity(uint16_t timeout=0);

	void delayWhileCheckingActivity(uint16_t delayMs, uint16_t activityCheckPeriod=250);


	/*
	 * Making requests.
	 *
	 * You can make "raw" requests by calling makeRequest()
	 * or use any of the helper methods below--mostly inlined such that
	 * there is no overhead anyway, it will just make your code
	 * clearer.
	 */
	bool makeRequest(DataLink::RequestCode reqCode);


	bool wakeUp();


	/* request shortcut methods
	 * Most of these offer two methods of access:
	 * xxxOn()/xxxOff()
	 * xxx(State::SetTo) -- so xxx(State::On) and xxx(State::Off)
	 *
	 */
	bool systemArm();
	bool systemDisarm();
	bool arm(State::SetTo onOrOff);

	bool groundOutOn();
	bool groundOutOff();
	bool groundOut(State::SetTo onOrOff);
	bool trunkRelease() ;
	bool panicOn();
	bool panicOff();
	bool panic(State::SetTo onOrOff) ;
	bool parklightOn();
	bool parklightOff();
	bool parklight(State::SetTo onOrOff);

	bool alarmOn() ;
	bool alarmOff() ;
	bool alarm(State::SetTo onOrOff);
	bool accessoryOn();
	bool accessoryOff() ;
	bool accessory(State::SetTo onOrOff);
	bool starterKillOn();
	bool starterKillOff() ;
	bool starterKill(State::SetTo onOrOff);


	// lock/unlock -- call specifying Driver::One or Driver::Two
	bool lock(Driver::Selection d=Driver::One) ;
	bool unlock(Driver::Selection d=Driver::One) ;
	bool priorityUnlock(Driver::Selection d) ;

	/*
	 * Methods offering Source options (from remote or from key), so are called with
	 * xxxOn(Source::Key) or xxxOn(Source::Remote)
	 *
	 */
	bool ignitionOn(Source::TriggerSource src);
	bool ignitionOff(Source::TriggerSource src) ;


	bool starterOn(Source::TriggerSource src);
	bool starterOff(Source::TriggerSource src);

	// ping/pong
	bool ping();
	bool pong();

	/* remote start/stop */
	bool remoteStart() ;
	bool remoteStop();
	bool remoteToggle();

	/*
	 *  horn activation -- send method on of:
	 *  	Horn::Off
	 *  	Horn::On
	 *  	Horn::Beep15ms
	 *  	Horn::Beep50ms
	 *  	Horn::Beep250ms
	 */
	bool horn(Horn::Action act) 	;

	/*
	 * auxiliary -- send one of the AUX_1, AUX_2 etc signals, by passing
	 * index between [1-8].
	 */
	bool auxiliary(uint8_t index); // AUX 1-8

	// the request simply performs the request -- it is asynchronous.
	// Your callbacks.requested_data_received callback
	// function will be called whenever the response comes in.
	bool requestTach();
	bool requestVSS();
	bool requestStatus();
	bool requestTemperature();

	/*
	 * The following getXXX() utility methods are, unlike their corresponding requestXXX() versions
	 * above, synchronous -- they will bypass using any callbacks.requested_data_received callback
	 * specified and will instead await a response for (up to) timeout ms and either return it
	 * or return -1 on failure.
	 */
	int16_t getTach(uint16_t timeout = SYNCHRONOUS_GETTER_DEFAULT_TIMEOUT_MS);
	int16_t getVSS(uint16_t timeout = SYNCHRONOUS_GETTER_DEFAULT_TIMEOUT_MS) ;
	int16_t getTemperature(uint16_t timeout =
			SYNCHRONOUS_GETTER_DEFAULT_TIMEOUT_MS);


	InputStatus getStatus(uint16_t timeout=SYNCHRONOUS_GETTER_DEFAULT_TIMEOUT_MS);

	int16_t synch_getter_value_received;


private:
	SerialSetup serial_setup;
	/* a few structure used internally */
	typedef struct DLMessageCodeToCallbackAssociationStruct {
		uint8_t raw_msg_code;
		GenericMessageHandler callback;
		DLMessageCodeToCallbackAssociationStruct(DataLink::MessageCode msg,
				GenericMessageHandler cb) :
				raw_msg_code((uint8_t) msg), callback(cb) {

		}

	} MessageCodeCallback;
	typedef int (*QueryResponseDataProcessor)(
			EvoLink::DataLink::RequestCode request, uint8_t returnedValue);
	typedef struct DLRequestWithResponseStruct {
		DataLink::RequestCode req;
		QueryResponseDataProcessor processor;
		DLRequestWithResponseStruct(DataLink::RequestCode r,
				QueryResponseDataProcessor massager = NULL) :
				req(r), processor(massager) {

		}
	} RequestWithResponse;

	/* associations between message codes and callbacks */
	static MessageCodeCallback callback_assocs[];
	/* list of requests that return a response */
	static RequestWithResponse reqs_with_responses[];


	bool parseMessage(int msg);
	MessageCodeCallback * codeToCallbackEntryFor(uint8_t raw_msg_code);

	RequestWithResponse * reqWithResponseEntryFor(DataLink::RequestCode c);

	void prepareRequestNeedingResponse(RequestWithResponse * setup);
	void resetPendingRequest();

	void sendRequest(DataLink::RequestCode reqCode);


	int16_t synchronousGet(uint16_t timeout, DataLink::RequestCode code);
	void synchronousValueGetterSetup();
	void synchronousValueGetterTeardown();

	RequestWithResponse * pending_request_response;
	uint32_t pending_request_issue_time;
#ifdef MINTIME_BETWEEN_WAKEUPS_MS
	uint32_t last_wakeup_time;
#endif
	uint32_t last_tx_time;
	uint8_t auto_delay_ms;

	QueryResponseHandler user_specified_handler; // used as swap space for synchronous getters


};

} /* namespace EvoLink */


// This is the main, global, EVO object, used
// in your code.
extern EvoLink::EvoAll EVO;


#endif /* DRIVER_H_ */
