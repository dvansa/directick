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

}//namespace os