/*
*
* Directick main program. Registers UserInput callbacks and exectues
* the message loop.
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
#include <iostream>

#include <directick/Actions.hpp>
#include <directick/UserInput.hpp>

//Close callback
void CallbackClose()
{
	PostQuitMessage(EXIT_SUCCESS);
}

void MessageLoop()
{
	UserInput ui;

	//Register actions:
	std::cout << "Registering actions..." << std::endl;
	//Ctrl & cursor on top-left corner -> Google search selected text
	ui.RegisterCallback(UserInput::KEYEVENT_PRESSED, UserInput::KEY_LCTRL, UserInput::KEYMOD_NONE, UserInput::ZONE_CORNER_TOPLEFT, action::SearchGoogleClipboardContent);
	//Ctrl & cursor on bottom-left corner -> Close program
	ui.RegisterCallback(UserInput::KEYEVENT_PRESSED, UserInput::KEY_LCTRL, UserInput::KEYMOD_NONE, UserInput::ZONE_CORNER_BOTTOMLEFT, CallbackClose);


	std::cout << "Running..." << std::endl;
	//Windows message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//Exit
		if (msg.message == WM_QUIT)
		{
			std::cout << "Closing" << std::endl;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

int main(int argc, char** argv)
{
	std::cout << "Directick - Additional OS shortcuts" << std::endl;
	MessageLoop();

	return 0;
}