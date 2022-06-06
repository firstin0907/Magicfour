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

	// app의 실행
	void run();

	// 게임 시작 전 도움말을 출력하는 부분
	bool run_before_game();

	// 게임이 동작하는 부분
	bool run_in_game();

	void make_new_file();

	// 게임이 종료되고 최고 기록 등이 출력되는 부분
	bool run_after_game();


	~App();
};

