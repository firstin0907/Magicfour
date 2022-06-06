#include "App.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include "FieldObject.h"
#include "Character.h"
#include "FontEffect.h"


#pragma warning(disable:4996)

App::App(std::string title, Vector2d<Uint32> window_size,
	Vector2d<Uint32> logical_size)
	: title_(title), initializer_(), time_manager_(), sound_manager_(),
	window_(title, window_size, "Resource\\Graphic\\Icon.ma4png"), renderer_(window_, logical_size)
{
}

void App::run()
{
	if (!run_before_game()) return;
	if (!run_in_game()) return;
	if (!run_after_game()) return;
}

bool App::run_before_game()
{
	SDL_Event event;
	bool running = true;

	renderer_.clear();

	renderer_.enroll_texture('U', 9, Point<int>(0, 0), 0, SDL_FLIP_NONE);
	renderer_.draw_rect(Range<int>({ 0, 0 }, { 1280, 720 }));
	renderer_.present();

	while (running)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return false;

			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_Z)
				{
					running = false;
				}
				break;
			}
		}
	}
	return true;
}

bool App::run_in_game()
{
	SDL_Event event;
	bool running = true;

	// 게임 시작에 앞서 처리
	time_manager_.update();
	object_manager_.start_spon(time_manager_.get_curr());

	// 음악 재생
	sound_manager_.play_music(0);

	// 인게임(캐릭터가 죽고 4초가 지나기 전까지 계속 반복)
	while (object_manager_.curr_character_->get_state() != FieldObject::DIE
		|| object_manager_.curr_character_->keep_state_time(time_manager_) < 4'000) {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				const int skill_number = object_manager_.curr_character_->proceed_input(event);

				if (skill_number != -1)
				{
					if (skill_number == 1) sound_manager_.play_sfx(1);
					else if (skill_number % 2 == 0) sound_manager_.play_sfx(2);
					else sound_manager_.play_sfx(3);

					object_manager_.change_character(skill_number);
				}
			}
		}
		time_manager_.update();

		renderer_.clear();
		renderer_.enroll_texture('U', 0, Point<int>(0, 0), 0, SDL_FLIP_NONE);
		renderer_.set_color(0, 0, 0, 165);
		renderer_.draw_rect(Range<int>({ 0, 0 }, { 1280, 720 }));
		renderer_.set_color(255, 255, 255, 255);

		object_manager_.update(time_manager_, renderer_, sound_manager_);
		object_manager_.move(time_manager_);
		object_manager_.draw(renderer_, time_manager_);

		renderer_.present();
	}

	sound_manager_.stop_music();
	return true;
}

void App::make_new_file()
{
	std::ofstream out("Resource\\Savedata\\rank.ma4sav", std::ios::binary);
	for (int i = 1; i <= 5; i++)
	{
		out << 10 - i << " 0" << std::endl;
	}
	out.close();
}

bool App::run_after_game()
{
	std::ifstream in("Resource\\Savedata\\rank.ma4sav", std::ios::binary);

	if (!in.is_open())
	{
		make_new_file();
		in = std::ifstream("Resource\\Savedata\\rank.ma4sav", std::ios::binary);
		if (!in.is_open())
		{
			throw std::exception("Failed to load rank file. ");
		}
	}

	std::time_t curr_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::pair<int, std::time_t> ranker_list[6];
	std::shared_ptr<Texture> ranking_texture[6][2];
	
	ranker_list[0].first = object_manager_.get_score();
	ranker_list[0].second = curr_time;


	for (int i = 1; i <= 5; i++)
	{
		if (!(in >> ranker_list[i].first))
		{
			in.close();
			make_new_file();
			throw std::exception("Failed to load rank file. New rank file has been created. ");
		}

		if (!(in >> ranker_list[i].second))
		{
			in.close();
			make_new_file();
			throw std::exception("Failed to load rank file. New rank file has been created. ");
		}
	}
	in.close();



	// [점수 - 시간]을 내림차순으로 정렬한다.
	std::sort(ranker_list, ranker_list + 6, std::greater<std::pair<int, std::time_t>>());
	
	// 정렬한 내용을 파일에 저장
	std::ofstream out("Resource\\Savedata\\rank.ma4sav", std::ios::binary);
	for (int i = 0; i < 5; i++)
	{
		out << ranker_list[i].first << " " << ranker_list[i].second << std::endl;
	}
	out.close();
		

	std::shared_ptr<Texture> title_texture = renderer_.make_text("HIGH SCORE", 0, WHITE);
	
	// 가운데 정렬
	title_texture->center.x = title_texture->size.x / 2;

	std::shared_ptr<Texture> credit_texture[4] =
	{
		renderer_.make_text("Game Made by Sin Gisub", 1, WHITE),
		renderer_.make_text("Graphic Drawn by Kim Jongkyeom, Sin Gisub", 1, WHITE),
		renderer_.make_text("Graphic Sourced by Pixabay", 1, WHITE),
		renderer_.make_text("Sound Sourced by Pixabay", 1, WHITE)
	};
	// 오른쪽 하단 정렬
	for (int i = 0; i < 4; i++) credit_texture[i]->center = credit_texture[i]->size;

	// 텍스쳐 만들기
	for (int i = 0; i < 6; i++)
	{
		SDL_Color curr_color = (ranker_list[i].second == curr_time) ? SDL_Color{255, 255, 0, 255} : WHITE;

		ranking_texture[i][0] = renderer_.make_text(std::to_string(ranker_list[i].first), 0, curr_color);
		ranking_texture[i][0]->center.x = ranking_texture[i][0]->size.x;
		
		std::stringstream ss;
		ss << std::put_time(std::localtime(&ranker_list[i].second), "%F %T");

		ranking_texture[i][1] = renderer_.make_text(ss.str(), 0, curr_color);
	}

	SDL_Event event;
	bool running = true;

	while (running)
	{
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return false;
			}
		}

		renderer_.clear();
		renderer_.enroll_texture('U', 0, Point<int>(0, 0), 0, SDL_FLIP_NONE);
		renderer_.set_color(0, 0, 0, 165);
		renderer_.draw_rect(Range<int>({ 0, 0 }, { 1280, 720 }));

		time_manager_.update();


		renderer_.enroll_texture(title_texture, { 1280 / 2, 170 }, 0, SDL_FLIP_NONE);
		
		for (int i = 0; i < 6; i++)
		{
			// 이번 게임에서 기록한 점수는 깜빡이는 효과
			if (ranker_list[i].second != curr_time || ((time_manager_ / 600) % 2))
			{
				renderer_.enroll_texture(ranking_texture[i][0], { 640 - 50, 250 + i * 50 }, 0, SDL_FLIP_NONE);
				renderer_.enroll_texture(ranking_texture[i][1], { 640, 250 + i * 50 }, 0, SDL_FLIP_NONE);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			// 크레딧 출력
			renderer_.enroll_texture(credit_texture[i], { 1260, 610 + i * 30 }, 0, SDL_FLIP_NONE);
		}
		renderer_.present();	
	}

	return false;
}

App::~App()
{

}