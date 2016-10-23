/*
*
* Common definitions.
*
*
* Copyright (C) 2016 Daniel van Sabben Alsina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
*/
#ifndef _DIRECTICK_COMMON
#define _DIRECTICK_COMMON

#define CSTR_PROGRAM_NAME "Directick"

#if defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS
#else
#error Platform not supported
#endif

//Windows registry values for startu running
#define CSTR_REGISTRY_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define CSTR_REGISTRY_SUBKEY CSTR_PROGRAM_NAME

#endif