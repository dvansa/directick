/*
*
* Actions.hpp implementations.
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
#include <directick/Actions.hpp>

namespace action
{

void SearchGoogleClipboardContent()
{
	//Ctrl + C
	UserInput::SimulateKey(UserInput::KEY_C, UserInput::KEYMOD_CTRL);

	os::ProgramSleep(2);

	//Get clipboard text
	std::string clipboard_str;
	os::GetClipboardText(clipboard_str);

	os::ProgramSleep(2);

	//Encode URL
	std::string http_str = EncodeToURL(clipboard_str);

	//CMD Google search
	std::string cmd_query = std::string("start www.google.com/?q=") + http_str;

	//Exec
	os::SysExecute(cmd_query.c_str());
}

}