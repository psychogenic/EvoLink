/*
 * codes.h -- Byte codes for EvoLink, part of the
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
 */

#ifndef EVOLINK_DATALINK_COMMANDS_H_
#define EVOLINK_DATALINK_COMMANDS_H_

// requests
#define REQ_DRIVER1_LOCK				0x10
#define REQ_DRIVER1_UNLOCK				0x11
#define REQ_SYSTEM_ARM					0x12
#define REQ_SYSTEM_DISARM				0x13
#define REQ_GROUNDOUT_ON				0x14
#define REQ_GROUNDOUT_OFF				0x15
#define REQ_TRUNK_RELEASE				0x16
#define REQ_PANIC_ON					0x17
#define REQ_PANIC_OFF					0x18
#define REQ_DRIVER2_LOCK				0x19
#define REQ_DRIVER2_UNLOCK				0x1A
#define REQ_PARKINGLIGHT_ON				0x1B
#define REQ_PARKINGLIGHT_OFF			0x1C
#define REQ_ALARM_ON					0x1D
#define REQ_ALARM_OFF					0x1E
#define REQ_ACCESSORY_ON				0x20
#define REQ_ACCESSORY_OFF				0x21
#define REQ_IGNITION_FROMREMOTE_ON		0x22
#define REQ_IGNITION_FROMREMOTE_OFF		0x23
#define REQ_STARTER_FROMREMOTE_ON		0x24
#define REQ_STARTER_FROMREMOTE_OFF		0x25
#define REQ_IGNITION_FROMKEY_ON			0x26
#define REQ_IGNITION_FROMKEY_OFF		0x27
#define REQ_STARTER_FROMKEY_ON			0x28
#define REQ_STARTER_FROMKEY_OFF			0x29
#define REQ_STARTERKILL_ON				0x2A
#define REQ_STARTERKILL_OFF				0x2B
#define REQ_HORN_ON						0x2E
#define REQ_HORN_OFF					0x2F
#define REQ_DRIVER1_PRIORITY_UNLOCK		0x1F
#define REQ_DRIVER2_PRIORITY_UNLOCK		0x2C
#define REQ_AUX_1						0x30
#define REQ_AUX_2						0x31
#define REQ_AUX_3						0x32
#define REQ_AUX_4						0x33
#define REQ_AUX_5						0x34
#define REQ_AUX_6						0x35
#define REQ_AUX_7						0x36
#define REQ_AUX_8						0x37
#define REQ_HORN_ON_250MS				0x3D
#define REQ_HORN_ON_50MS				0x3E
#define REQ_HORN_ON_15MS				0x3F
#define REQ_REQUEST_TEMPERATURE			0x40
#define REQ_PING						0x41
#define REQ_PONG						0x42
#define REQ_REQUEST_TACH				0x43
#define REQ_REQUEST_VSS					0x44
#define REQ_REQUEST_INPUT				0x45
#define REQ_WAKEUP						0x46
#define REQ_REMOTE_START				0x47
#define REQ_REMOTE_STOP					0x48
#define REQ_REMOTE_TOGGLE				0x49

// messages
#define MSG_REMOTESTARTER_DISARM		0x38
#define MSG_REMOTESTARTER_ARM			0x39
#define MSG_REMOTESTARTER_ON			0x3A
#define MSG_REMOTESTARTER_OFF			0x3B
#define MSG_REMOTESTARTER_UNLOCKDISARM	0x3C
#define MSG_REMOTESTARTER_LOCKARM		0x3D
#define MSG_DOOR_OPENED					0x50
#define MSG_DOOR_CLOSED					0x51
#define MSG_HOOD_OPENED					0x52
#define MSG_HOOD_CLOSED					0x53
#define MSG_TRUNK_OPENED				0x54
#define MSG_TRUNK_CLOSED				0x55
#define MSG_HANDBRAKE_ON				0x56
#define MSG_HANDBRAKE_OFF				0x57
#define MSG_BRAKE_ON					0x58
#define MSG_BRAKE_OFF					0x59
#define MSG_TACH_ON						0x5A
#define MSG_TACH_OFF					0x5B
#define MSG_TACH_OVERREV				0x5C
#define MSG_VSS_OVER_15MPH				0x5D
#define MSG_SHOCKSENSOR_TRIGGER			0x5E
#define MSG_ALARMSENSOR_PREWARN			0x5F
#define MSG_TEMPERATURE_ERROR			0xFF
#define MSG_PING						0x60
#define MSG_PONG						0x61
#define MSG_TILTSENSOR_TRIGGER			0x62
#define MSG_CARKEY_IN_ON				0x67
#define MSG_CARKEY_IN_OFF				0x68
#define MSG_REMOTEPROGRAMMING_ENABLE	0x69
#define MSG_REMOTEPROGRAMMING_DISABLE 	0x6A








namespace EvoLink {

namespace DataLink {


	typedef enum DLRequestEnum {
		Driver1_Lock = REQ_DRIVER1_LOCK,
		Driver1_Unlock = REQ_DRIVER1_UNLOCK,
		System_Arm = REQ_SYSTEM_ARM,
		System_Disarm = REQ_SYSTEM_DISARM,
		GroundOut_On = REQ_GROUNDOUT_ON,
		GroundOut_Off = REQ_GROUNDOUT_OFF,
		Trunk_Release = REQ_TRUNK_RELEASE,
		Panic_On = REQ_PANIC_ON,
		Panic_Off = REQ_PANIC_OFF,
		Driver2_Lock = REQ_DRIVER2_LOCK,
		Driver2_Unlock = REQ_DRIVER2_UNLOCK,
		ParkingLight_On = REQ_PARKINGLIGHT_ON,
		ParkingLight_Off = REQ_PARKINGLIGHT_OFF,
		Alarm_On = REQ_ALARM_ON,
		Alarm_Off = REQ_ALARM_OFF,
		Accessory_On = REQ_ACCESSORY_ON,
		Accessory_Off = REQ_ACCESSORY_OFF,
		Ignition_FromRemote_On = REQ_IGNITION_FROMREMOTE_ON,
		Ignition_FromRemote_Off = REQ_IGNITION_FROMREMOTE_OFF,
		Starter_FromRemote_On = REQ_STARTER_FROMREMOTE_ON,
		Starter_FromRemote_Off = REQ_STARTER_FROMREMOTE_OFF,
		Ignition_FromKey_On = REQ_IGNITION_FROMKEY_ON,
		Ignition_FromKey_Off = REQ_IGNITION_FROMKEY_OFF,
		Starter_FromKey_On = REQ_STARTER_FROMKEY_ON,
		Starter_FromKey_Off = REQ_STARTER_FROMKEY_OFF,
		StarterKill_On = REQ_STARTERKILL_ON,
		StarterKill_Off = REQ_STARTERKILL_OFF,
		Horn_On = REQ_HORN_ON,
		Horn_Off = REQ_HORN_OFF,
		Driver1_Priority_Unlock = REQ_DRIVER1_PRIORITY_UNLOCK,
		Driver2_Priority_Unlock = REQ_DRIVER2_PRIORITY_UNLOCK,
		AUX_1 = REQ_AUX_1,
		AUX_2 = REQ_AUX_2,
		AUX_3 = REQ_AUX_3,
		AUX_4 = REQ_AUX_4,
		AUX_5 = REQ_AUX_5,
		AUX_6 = REQ_AUX_6,
		AUX_7 = REQ_AUX_7,
		AUX_8 = REQ_AUX_8,
		Horn_On_250ms = REQ_HORN_ON_250MS,
		Horn_On_50ms  = REQ_HORN_ON_50MS,
		Horn_On_15ms  = REQ_HORN_ON_15MS,
		Request_Temperature = REQ_REQUEST_TEMPERATURE,
		Ping_Request = REQ_PING,
		Pong_Request = REQ_PONG,
		Request_Tach = REQ_REQUEST_TACH,
		Request_VSS = REQ_REQUEST_VSS,
		Request_Input = REQ_REQUEST_INPUT,
		WakeUp = REQ_WAKEUP,
		Remote_Start = REQ_REMOTE_START,
		Remote_Stop = REQ_REMOTE_STOP,
		Remote_Toggle = REQ_REMOTE_TOGGLE

	} RequestCode;


	typedef enum DLMessageEnum {

		RemoteStarter_Disarm 	 		= MSG_REMOTESTARTER_DISARM,
		RemoteStarter_Arm 	 			= MSG_REMOTESTARTER_ARM,
		RemoteStarter_On 	 			= MSG_REMOTESTARTER_ON,
		RemoteStarter_Off 	 			= MSG_REMOTESTARTER_OFF,
		RemoteStarter_UnlockDisarm 		= MSG_REMOTESTARTER_UNLOCKDISARM,
		RemoteStarter_LockArm 	 		= MSG_REMOTESTARTER_LOCKARM,

		Door_Opened 	 				= MSG_DOOR_OPENED,
		Door_Closed 	 				= MSG_DOOR_CLOSED,
		Hood_Opened 	 				= MSG_HOOD_OPENED,
		Hood_Closed 	 				= MSG_HOOD_CLOSED,
		Trunk_Opened 	 				= MSG_TRUNK_OPENED,
		Trunk_Closed 	 				= MSG_TRUNK_CLOSED,

		HandBrake_On 	 				= MSG_HANDBRAKE_ON,
		HandBrake_Off 					= MSG_HANDBRAKE_OFF,
		Brake_On 	 					= MSG_BRAKE_ON,
		Brake_Off 	 					= MSG_BRAKE_OFF,

		Tach_On 	 					= MSG_TACH_ON,
		Tach_Off 	 					= MSG_TACH_OFF,
		Tach_OverRev 				 	= MSG_TACH_OVERREV,

		VSS_Over_15MPH 	 				= MSG_VSS_OVER_15MPH,
		ShockSensor_Trigger 	 		= MSG_SHOCKSENSOR_TRIGGER,
		AlarmSensor_PreWarn 	 		= MSG_ALARMSENSOR_PREWARN,


		Ping_Message					= MSG_PING,
		Pong_Message					= MSG_PONG,
		TiltSensor_Trigger 	 			= MSG_TILTSENSOR_TRIGGER,
		CarKey_In_On 	 				= MSG_CARKEY_IN_ON,
		CarKey_In_Off 	 				= MSG_CARKEY_IN_OFF,
		RemoteProgramming_Enable 	 	= MSG_REMOTEPROGRAMMING_ENABLE,
		RemoteProgramming_Disable		= MSG_REMOTEPROGRAMMING_DISABLE,



		Temperature_Error 	 			= MSG_TEMPERATURE_ERROR

	} MessageCode ;
} /* namespace DataLink */

// function-specific groups
// These definitions are used to create sub-groups of related events.
// It is important to share identical DataLink codes for the enum values
// as these will be assumed to match to allow safe and "cheap" conversion between types.

namespace RemoteStarter {
typedef enum DLRemoteStarterEventEnum {
	Disarm = MSG_REMOTESTARTER_DISARM,
	Arm = MSG_REMOTESTARTER_ARM,
	On = MSG_REMOTESTARTER_ON,
	Off = MSG_REMOTESTARTER_OFF,
	UnlockDisarm = MSG_REMOTESTARTER_UNLOCKDISARM,
	LockArm = MSG_REMOTESTARTER_LOCKARM
} Event;
}
namespace OpenClose {
typedef enum DLOpenCloseEventEnum {
	Door_Opened = MSG_DOOR_OPENED,
	Door_Closed = MSG_DOOR_CLOSED,
	Hood_Opened = MSG_HOOD_OPENED,
	Hood_Closed = MSG_HOOD_CLOSED,
	Trunk_Opened = MSG_TRUNK_OPENED,
	Trunk_Closed = MSG_TRUNK_CLOSED
} Event;
}

namespace Brake {
typedef enum DLBrakeEventEnum {
	HandBrake_On = MSG_HANDBRAKE_ON,
	HandBrake_Off = MSG_HANDBRAKE_OFF,
	On = MSG_BRAKE_ON,
	Off = MSG_BRAKE_OFF
} Event;
}

namespace Sensor {
typedef enum DLSensorEvent {
	Shock = MSG_SHOCKSENSOR_TRIGGER,
	Alarm_PreWarn = MSG_ALARMSENSOR_PREWARN,
	Tilt = MSG_TILTSENSOR_TRIGGER,
	Over_15MPH = MSG_VSS_OVER_15MPH
} Event;
}

namespace Tach {
typedef enum DLTachEvent {
	On = MSG_TACH_ON,
	Off = MSG_TACH_OFF,
	OverRev = MSG_TACH_OVERREV
} Event;
}

namespace ErrorMessage {

typedef enum DLErrorEvent {
	UnsupportedValue = 0,
	RequestTimeout,
	Temperature_Error = MSG_TEMPERATURE_ERROR

} Event ;
}

} /* namespace EvoLink */




#endif /* EVOLINK_DATALINK_COMMANDS_H_ */
