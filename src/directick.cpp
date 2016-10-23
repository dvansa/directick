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

int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int cmdShow)
{
	//Get Arguments
	LPWSTR * wargv;
	int argc;
	std::string exe_path, mode;

	wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (wargv == NULL || argc < 1)
	{
		MessageBoxA(NULL, "Error parsing program arguments.", CSTR_PROGRAM_NAME, MB_OK);
		return 1;
	}

	//Arg[1]:execution mode = <NONE>,INSTALL,UNINSTALL
	if (argc > 1)
	{
		std::wstring wstr;
		wstr = std::wstring(wargv[1]);
		mode = std::string(wstr.begin(), wstr.end());
		mode = StrToUppercase(mode);
	}
	LocalFree(wargv);

	//Install on startup (method: registry 'Run' windows key)
	if (mode == "INSTALL" || mode == "")
	{
		if (!os::RegistrySubkeyExists(CSTR_REGISTRY_KEY, CSTR_REGISTRY_SUBKEY))
		{
			//First time running -> Install: run at startup

			//Get executable path
			LPSTR cstr_name = new char[MAX_PATH_LEN];
			int len = GetModuleFileNameA(NULL, cstr_name, MAX_PATH_LEN);
			if (len == 0)
			{
				MessageBoxA(NULL, "Error getting executable path.", CSTR_PROGRAM_NAME, MB_OK);
				return 1;
			}
			cstr_name[len] = '\0';

			exe_path = std::string(cstr_name);

			if (cstr_name)
			{
				delete cstr_name;
			}

			//Install to register
			std::string exe_path_value = "\"" + exe_path + "\"";
			if (os::RegistryAddSubkey(CSTR_REGISTRY_KEY, CSTR_REGISTRY_SUBKEY, exe_path_value.c_str()))
			{
				std::string message = std::string(CSTR_PROGRAM_NAME).append(" has been installed successfully and will run at computer startup.");
				MessageBoxA(NULL, message.c_str(), CSTR_PROGRAM_NAME, MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "Failed to update registry.", CSTR_PROGRAM_NAME, MB_OK);
				return 1;
			}
		}
	}

	if (mode == "UNINSTALL")
	{
		if (os::RegistrySubkeyExists(CSTR_REGISTRY_KEY, CSTR_REGISTRY_SUBKEY))
		{
			if (os::RegisterRemoveSubkey(CSTR_REGISTRY_KEY, CSTR_REGISTRY_SUBKEY))
			{
				std::string message = std::string(CSTR_PROGRAM_NAME).append(" has been removed from startup programs.");
				MessageBoxA(NULL, message.c_str(), CSTR_PROGRAM_NAME, MB_OK);
			}
			else
			{
				MessageBoxA(NULL, "Failed to update registry.", CSTR_PROGRAM_NAME, MB_OK);
			}
		}
		else
		{
			std::string message = std::string(CSTR_PROGRAM_NAME).append(" it is not installed.");
			MessageBoxA(NULL, message.c_str(), CSTR_PROGRAM_NAME, MB_OK);
		}
	}
	
	//Exit after completing operation
	if (mode == "INSTALL" || mode == "UNINSTALL")
	{
		return 0;
	}

	std::cout << "Directick - Additional OS shortcuts" << std::endl;

	//Program loop
	MessageLoop();

	return 0;
}
