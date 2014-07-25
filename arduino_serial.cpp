/*
 * arduino_serial.cpp-- Arduino SerialSetup for EvoLink,
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
 */

#include "includes/serial.h"

#ifdef PLATFORM_ARDUINO

namespace EvoLink {

HardwareSerial * _serialconn_usart = NULL;

void SerialConnection::setup(SerialSetup & params)
{
	_serialconn_usart = params.usart;

	if (params.do_begin)
	{
		_serialconn_usart->begin(params.baud_rate, params.config);
	}
}

size_t SerialConnection::write(uint8_t c)
{
	return _serialconn_usart->write(c);

}

int SerialConnection::available()
{
	return _serialconn_usart->available();
}

int SerialConnection::read()
{
	return _serialconn_usart->read();
}

} /* namespace EvoLink */


#endif /* PLATFORM_ARDUINO */

