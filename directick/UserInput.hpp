/*
*
* Manages user interaction with Keyboard/Mouse and register
* callbacks associated to specific triggers.
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

#ifndef _DIRECTICK_USER_INPUT
#define _DIRECTICK_USER_INPUT

#include <directick/common/common.hpp>
#include <directick/common/OSUtils.hpp>
#include <directick/common/GeneralUtils.hpp>

#include <map>

class UserInput
{
public:

	//Key codes
	enum Key
	{
		KEY_NONE,
		KEY_LCTRL,
		KEY_C
	};

	//Modifer key codes
	enum KeyMod
	{
		KEYMOD_NONE,
		KEYMOD_CTRL,
		KEYMOD_ALT
	};
	
	//Keyboard events
	enum KeyboardEvent
	{
		KEYEVENT_NONE,
		KEYEVENT_PRESSED,
		KEYEVENT_RELEASED
	};

	//Cursor positional zones
	enum CursorZone
	{
		ZONE_NONE,
		ZONE_CORNER_TOPLEFT,
		ZONE_CORNER_TOPRIGHT,
		ZONE_CORNER_BOTTOMLEFT,
		ZONE_CORNER_BOTTOMRIGHT
	};

	UserInput();
	~UserInput();

	//Callback triggers : (event, key, key modifer, cursor zone)
	typedef std::tuple<KeyboardEvent, Key, KeyMod, CursorZone> CallbackTrigger;
	typedef void (* UserCallback)();
	//Register a callback to be exectued when trigger happens
	void RegisterCallback(KeyboardEvent event, Key key,KeyMod keymod, CursorZone zone, UserCallback callback);

	//Simulate key stroke with optional modifer
	static void SimulateKey(Key key, KeyMod keymod);

private:

	//Parameters
	//Time barrier between callbacks execution
	long long barrier_time_ms = 500;
	//Desktop CursorZoze size in percentage of width
	float zone_size = 0.08f;// 8% of width

	//Last action timestamp
	long long last_action_ts;

	//Gets current cursor zone
	CursorZone GetCursorZone() const;
	//Get KeyMod
	KeyMod GetKeyMod() const;
	//Translate windows VirtualKeys
	Key TranslateWinKey(DWORD vk) const;
	//Translate windows events
	KeyboardEvent TranslateWinEvent(WPARAM event) const;

	// CallbackTrigger -> Callback association
	std::map< CallbackTrigger, UserCallback> callback_list;

	//Windows: hook keyboard
	static LRESULT CALLBACK GlobalHookProc(int code, WPARAM wParam, LPARAM lParam)
	{
		return ui_ptr->HookKeyboardCallback(code, wParam, lParam);
	}
	LRESULT HookKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK handle_keyboard_hook;
	static UserInput * ui_ptr;
	//
};

#endif