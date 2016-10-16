/*
*
* UserInput class implementation.
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
#include <directick/UserInput.hpp>

HHOOK UserInput::handle_keyboard_hook = nullptr;
UserInput * UserInput::ui_ptr = nullptr;

void UserInput::SimulateKey(UserInput::Key key, UserInput::KeyMod keymod = UserInput::KEYMOD_NONE)
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	WORD vk_mod, vk_key;

	//Send modifier press
	if (keymod != KEYMOD_NONE)
	{
		if (keymod == KEYMOD_CTRL) vk_mod = VK_CONTROL;
		else if (keymod == KEYMOD_ALT) vk_mod = VK_MENU;

		ip.ki.wVk = vk_mod;
		ip.ki.dwFlags = 0; //key press
		SendInput(1, &ip, sizeof(INPUT));
	}

	//Send key press and release
	if (keymod != KEY_NONE)
	{
		if (key == KEY_C) vk_key = 'C';
		//else if
		//...

		ip.ki.wVk = vk_key;
		ip.ki.dwFlags = 0; //key press
		SendInput(1, &ip, sizeof(INPUT));


		ip.ki.wVk = vk_key;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}

	//Send modifier release
	if (keymod != KEYMOD_NONE)
	{

		ip.ki.wVk = vk_mod;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

void UserInput::RegisterCallback(KeyboardEvent event, Key key, KeyMod keymod, CursorZone zone, UserCallback callback)
{
	callback_list[CallbackTrigger(event, key, keymod, zone)] = callback;
}

//Hook
LRESULT UserInput::HookKeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:

			//Time barrier between actions
			if (GetCurrentMilliseconds() > last_action_ts + barrier_time_ms)
			{
				last_action_ts = GetCurrentMilliseconds();

				PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

				//Get current cursor position zone, key, keymod
				KeyboardEvent key_event = TranslateWinEvent(wParam);
				Key key = TranslateWinKey(p->vkCode);
				KeyMod key_mod = GetKeyMod();
				CursorZone zone = GetCursorZone();

				//Execute callback if registered
				CallbackTrigger trigger(key_event, key, key_mod, zone);
				if (callback_list.find(trigger) != callback_list.end())
				{
					callback_list[trigger]();
				}

				break;
			}
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}



UserInput::UserInput()
{
	//Static instance ptr and keyboard hook setup
	ui_ptr = this;
	handle_keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, GlobalHookProc, 0, 0);

	if (handle_keyboard_hook == nullptr || ui_ptr == nullptr)
	{
		throw std::exception("Only single instance of UserInput is permited.");
	}
}

UserInput::~UserInput()
{

	//Release keyboard hook
	if (UserInput::handle_keyboard_hook)
	{
		UnhookWindowsHookEx(UserInput::handle_keyboard_hook);
		UserInput::handle_keyboard_hook = nullptr;
	}

	//Release static ptr
	ui_ptr = nullptr;
}

UserInput::CursorZone UserInput::GetCursorZone() const
{
	int x, y, w, h;
	os::GetCursorPosition(x, y);
	os::GetDesktopDimensions(w, h);

	//Size of corner zone in pixels
	int zone_px = (int)((float)w*zone_size);

	//Cursor collision
	if (x < zone_px && y < zone_px) return CursorZone::ZONE_CORNER_TOPLEFT;
	else if (x >(w - zone_px) && y < zone_px) return CursorZone::ZONE_CORNER_TOPRIGHT;
	else if (x < zone_px && y >(h - zone_px)) return CursorZone::ZONE_CORNER_BOTTOMLEFT;
	else if (x >(w - zone_px) && y >(h - zone_px)) return CursorZone::ZONE_CORNER_BOTTOMLEFT;
	else return CursorZone::ZONE_NONE;
}

UserInput::KeyboardEvent UserInput::TranslateWinEvent(WPARAM event) const
{
	if (event == WM_KEYDOWN || event == WM_SYSKEYDOWN) return KeyboardEvent::KEYEVENT_PRESSED;
	else if (event == WM_KEYUP || event == WM_SYSKEYUP) return KeyboardEvent::KEYEVENT_RELEASED;
	else return KeyboardEvent::KEYEVENT_NONE;
}

UserInput::Key UserInput::TranslateWinKey(DWORD vk) const
{
	if (vk == VK_LCONTROL) return KEY_LCTRL;
	else if (vk == 'C') return KEY_C;
	return KEY_NONE;
}

UserInput::KeyMod UserInput::GetKeyMod() const
{
	int key_state;

	//Control
	key_state = GetAsyncKeyState(VK_CONTROL);
	if (key_state) return KeyMod::KEYMOD_CTRL;

	//Alt
	key_state = GetAsyncKeyState(VK_MENU);
	if (key_state) return KeyMod::KEYMOD_ALT;

	//(Else)
	return KeyMod::KEYMOD_NONE;
}