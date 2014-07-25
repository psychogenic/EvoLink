/*
 * types.h -- Type definitions for EvoLink, part of the
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
 * This file contains the definitions for the various callback
 * signatures and enumerated types used by EvoLink.
 *
 */

#ifndef EVOLINK_TYPES_H_
#define EVOLINK_TYPES_H_

#include "datalink/codes.h"

typedef void (*GenericMessageHandler)(EvoLink::DataLink::MessageCode message);
typedef void (*QueryResponseHandler)(EvoLink::DataLink::RequestCode request,
		int value);

typedef void (*RemoteStarterEventHandler)(EvoLink::RemoteStarter::Event event);
typedef void (*OpenCloseEventHandler)(EvoLink::OpenClose::Event event);
typedef void (*BrakeEventHandler)(EvoLink::Brake::Event event);
typedef void (*SensorEventHandler)(EvoLink::Sensor::Event event);
typedef void (*TachEventHandler)(EvoLink::Tach::Event event);
typedef void (*ErrorEventHandler)(EvoLink::ErrorMessage::Event event,
		uint8_t param);

namespace EvoLink {

namespace Horn {

typedef enum HornActionEnum {
	Off = 0,
	On,
	Beep15ms,
	Beep50ms,
	Beep250ms
} Action;

}

namespace Source {

typedef enum TriggerSourceEnum {
	Remote = 0,
	Key

} TriggerSource;

}

namespace State {

typedef enum StateSetToEnum {
	Off = 0,
	On
} SetTo;

typedef enum StateOpenCloseEnum {

	Closed = 0,
	Open = 1
} OpenClose;

}


namespace Driver {
typedef enum {
	One=1,
	Two=2
} Selection;

}

typedef struct InputStatusStruct {

	State::OpenClose door;
	State::OpenClose hood;
	State::OpenClose trunk;
	State::SetTo	 tach;
	State::SetTo	 hand_brake;
	State::SetTo	 brake;

	bool valid;
	InputStatusStruct() :
		door(State::Open),
		hood(State::Open),
		trunk(State::Open),
		tach(State::On),
		hand_brake(State::Off),
		brake(State::Off),
		valid(false)
	{

	}

	InputStatusStruct(uint8_t st) :
		valid(true)
	{
		door =  	 (st & (1<<0) ) ? State::Open : State::Closed;
		hood = 		 (st & (1<<1) ) ? State::Open : State::Closed;
		trunk = 	 (st & (1<<2) ) ? State::Open : State::Closed;

		tach = 		 (st & (1<<3) ) ? State::On : State::Off;
		hand_brake = (st & (1<<4) ) ? State::On : State::Off;
		brake = 	 (st & (1<<5) ) ? State::On : State::Off;
	}

	uint8_t asByte() {
		uint8_t b;

		b = (door == State::Open) ? 1 : 0;

		b |= (((hood == State::Open) ? 1 : 0) << 1);
		b |= (((trunk == State::Open) ? 1 : 0) << 2);

		b |= (((tach == State::On) ? 1 : 0) << 3);
		b |= (((hand_brake == State::On) ? 1 : 0) << 4);
		b |= (((brake == State::On) ? 1 : 0) << 5);

		return b;

	}



} InputStatus;



typedef struct CallbackContainerStruct {

	// receive EvoLink::RemoteStarter::Events
	RemoteStarterEventHandler 	remotestarter_event;
	// receive EvoLink::OpenClose::Events
	OpenCloseEventHandler		openclose_event;
	// receive EvoLink::Brake::Events
	BrakeEventHandler			brake_event;
	// receive EvoLink::Sensor::Events
	SensorEventHandler 			sensor_event;
	// receive EvoLink::Tach::Events
	TachEventHandler 			tach_event;
	// receive EvoLink::DataLink::MessageCodes for messages not handled by above
	GenericMessageHandler		message_received;

	// Request Data callback: function with signature
	// void ... (EvoLink::DataLink::RequestCode request, int value)
	// which will be called when temperature/input/tach/vss request
	// responses are received
	QueryResponseHandler		requested_data_received;

	// receive EvoLink::ErrorMessage::Events
	ErrorEventHandler			error_event;

	CallbackContainerStruct() :
		remotestarter_event(NULL),
		openclose_event(NULL),
		brake_event(NULL),
		sensor_event(NULL),
		tach_event(NULL),
		message_received(NULL),
		requested_data_received(NULL),
		error_event(NULL)
	{

	}

} CallbackContainer;

}

#endif /* TYPES_H_ */
