#pragma once
#include "header.h"

class KeyboardManager
{
private:
	// KEY_DOWN�̳� KEY_UP���� state�� �ٲ� Ű�����
	std::vector<SDL_Scancode> code_changed_;

public:
	// (state_�� KEY_DOWN or KEY_DOWN_NOW�� ��) �ش� Ű�� ���������� ���� �ð�.
	// (state_�� KEY_UP �� ��) �ش� Ű�� ���ȴ� �ð�
	// ������ �ʾҴٸ� 0
	Uint32	press_time_[513];

	// �ش� Ű������ ����
	enum
	{
		KEY_DOWN_NOW, KEY_DOWN, KEY_UP_NOW, KEY_IDLE
	} state_[513];


public:
	KeyboardManager();

	// SDL_event�� �޾Ƽ� state_ ���� ����
	// event���� SDL_
	void set_from_event(const SDL_Event& event, const TimeManager& tm);

	Uint32 get_press_time(const SDL_Scancode& code);

	// �����Ӹ��� 1ȸ ����, keyboard_changed_state �ʱ�ȭ
	void clear();

	~KeyboardManager();
};

