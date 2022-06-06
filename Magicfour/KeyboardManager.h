#pragma once
#include "header.h"

class KeyboardManager
{
private:
	// KEY_DOWN이나 KEY_UP으로 state가 바뀐 키보드들
	std::vector<SDL_Scancode> code_changed_;

public:
	// (state_가 KEY_DOWN or KEY_DOWN_NOW일 때) 해당 키가 마지막으로 눌린 시간.
	// (state_가 KEY_UP 일 때) 해당 키가 눌렸던 시간
	// 눌리지 않았다면 0
	Uint32	press_time_[513];

	// 해당 키보드의 상태
	enum
	{
		KEY_DOWN_NOW, KEY_DOWN, KEY_UP_NOW, KEY_IDLE
	} state_[513];


public:
	KeyboardManager();

	// SDL_event를 받아서 state_ 변수 변경
	// event에는 SDL_
	void set_from_event(const SDL_Event& event, const TimeManager& tm);

	Uint32 get_press_time(const SDL_Scancode& code);

	// 프레임마다 1회 실행, keyboard_changed_state 초기화
	void clear();

	~KeyboardManager();
};

