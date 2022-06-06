#include "KeyboardManager.h"
#include "global.h"

KeyboardManager::KeyboardManager()
{
}

void KeyboardManager::set_from_event(const SDL_Event& event, const TimeManager& tm)
{
	const SDL_Scancode& code = event.key.keysym.scancode;

	if (event.type == SDL_KEYDOWN)
	{
		if (state_[code] == KEY_DOWN || state_[code] == KEY_DOWN_NOW)
		{
			return;
		}
		state_[code] = KEY_DOWN_NOW;
		press_time_[code] = tm.get_curr();
	}
	else
	{
		state_[code] = KEY_UP_NOW;
		press_time_[code] = tm.get_curr() - press_time_[code];
	}
	code_changed_.push_back(code);
}

void KeyboardManager::clear()
{
	for (auto &code : code_changed_)
	{
		if (state_[code] == KEY_DOWN_NOW)
		{
			state_[code] = KEY_DOWN;
		}
		else if(state_[code] == KEY_UP_NOW)
		{
			state_[code] = KEY_IDLE;
		}
	}
	code_changed_.clear();
}

KeyboardManager::~KeyboardManager()
{
}
