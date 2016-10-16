/*
*
* General utility functions.
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
#ifndef _DIRECTICK_GENERAL_UTILS
#define _DIRECTICK_GENERAL_UTILS

#include <string>
#include <chrono>

//Get current time in total milliseconds
long long GetCurrentMilliseconds();

//Encodes string to URL
std::string EncodeToURL(const std::string & str);

#endif