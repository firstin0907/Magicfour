#pragma once
#include "header.h"

#include "Initializer.h"
#include "Window.h"
#include "Renderer.h"
#include "TimeManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"

class App
{
	Initializer		initializer_;
	Window			window_;
	Renderer		renderer_;
	TimeManager		time_manager_;
	SoundManager	sound_manager_;
	ObjectManager	object_manager_;
	

	std::string		title_;

public:
	App(std::string title = "Game Title", Vector2d<Uint32> window_size = { 800, 600 },
		Vector2d<Uint32> logical_size = { 0, 0 });

	// app�� ����
	void run();

	// ���� ���� �� ������ ����ϴ� �κ�
	bool run_before_game();

	// ������ �����ϴ� �κ�
	bool run_in_game();

	void make_new_file();

	// ������ ����ǰ� �ְ� ��� ���� ��µǴ� �κ�
	bool run_after_game();


	~App();
};

