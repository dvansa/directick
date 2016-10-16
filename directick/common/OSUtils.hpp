/*
*
* Operating system dependent utility functions.
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
#ifndef _DIRECTICK_OS_UTILS
#define _DIRECTICK_OS_UTILS

#include <Windows.h>
#include <string>
#include <iostream>

namespace os
{

//	Windows
///////////////////////////////

//Sleep
void ProgramSleep(long long milliseconds);

//Get text string in clipboard
bool GetClipboardText(std::string & clipboard_text);

//Get Desktop width, height
void GetDesktopDimensions(int & width, int & height);

//Get cursor position in desktop
void GetCursorPosition(int & x, int & y);


}//namespace os

#endif