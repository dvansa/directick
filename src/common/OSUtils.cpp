/*
*
* OSUtils.hpp implementations.
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
#include <directick/common/OSUtils.hpp>

namespace os
{
	void SysExecute(const char * command)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		LPCSTR cmd = "C:\\Windows\\System32\\cmd.exe";
		std::string args = std::string("cmd /C ") + std::string(command);
		LPSTR exec = new char[args.length() + 1];
		strcpy_s(exec,args.length()+1, args.c_str());

		if (CreateProcessA(cmd, exec, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess,INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		if (exec)
		{
			delete exec;
		}
	}

	void ProgramSleep(long long milliseconds)
	{
		Sleep((DWORD)milliseconds);
	}

	bool GetClipboardText(std::string & clipboard_text)
	{
		//Open clipboard
		if (!OpenClipboard(nullptr))
		{
			return false; //Can not open clipboard
		}

		//Handle to text object
		HANDLE h_data = GetClipboardData(CF_TEXT);
		if (h_data == nullptr)//Can not retrive data
		{
			CloseClipboard();
			return false;
		}

		//Retrieve cstr pointer
		char * ptr_text = static_cast<char*>(GlobalLock(h_data));
		if (ptr_text == nullptr)
		{
			GlobalUnlock(h_data);
			CloseClipboard();
			return false;
		}

		clipboard_text.assign(ptr_text);

		//Release lock and close clipboard
		GlobalUnlock(h_data);
		CloseClipboard();

		return true;
	}

	void GetDesktopDimensions(int & width, int & height)
	{
		RECT desktop_rect;

		//Retrieve desktop handler
		const HWND handle_desktop = GetDesktopWindow();

		//Get window rect
		GetWindowRect(handle_desktop, &desktop_rect);

		width = desktop_rect.right;
		height = desktop_rect.bottom;
	}

	void GetCursorPosition(int & x, int & y)
	{
		POINT cursor;

		GetCursorPos(&cursor);

		x = cursor.x;
		y = cursor.y;
	}

	bool RegistrySubkeyExists(const std::string & key, const std::string & subkey)
	{
		if (RegGetValueA(HKEY_CURRENT_USER, key.c_str(), subkey.c_str(), RRF_RT_ANY, NULL,NULL,NULL) != ERROR_SUCCESS)
		{
			return false;
		}
		return true;
	}

	bool RegistryAddSubkey(const std::string & key, const std::string & subkey, const std::string & value)
	{
		HKEY reg_value;
		if (RegOpenKey(HKEY_CURRENT_USER, key.c_str(), &reg_value) != ERROR_SUCCESS)
		{
			return false;
		}

		if (RegSetValueEx(reg_value, TEXT(subkey.c_str()), 0, REG_SZ, (LPBYTE)value.c_str(), (DWORD) (value.length()+1) ) != ERROR_SUCCESS)
		{
			RegCloseKey(reg_value);
			return false;
		}
		RegCloseKey(reg_value);
		return true;
	}

	bool RegisterRemoveSubkey(const std::string & key, const std::string & subkey)
	{
		HKEY reg_value;
		if (RegOpenKey(HKEY_CURRENT_USER, TEXT(key.c_str()), &reg_value) != ERROR_SUCCESS)
		{
			return false;
		}

		if (RegDeleteValue(reg_value, TEXT(subkey.c_str())) != ERROR_SUCCESS)
		{
			RegCloseKey(reg_value);
			return false;
		}
		RegCloseKey(reg_value);
		return true;
	}

}//namespace os