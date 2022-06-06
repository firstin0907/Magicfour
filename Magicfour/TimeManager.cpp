#include "TimeManager.h"
#include "global.h"

TimeManager::TimeManager()
{
	curr_time_ = prev_time_ = SDL_GetTicks(), d_time_ = 0;
}

void TimeManager::update()
{
	prev_time_ = curr_time_;
	curr_time_ = SDL_GetTicks();
	d_time_ = curr_time_ - prev_time_;
}

Uint32 TimeManager::get_curr() const
{
	return curr_time_;
}

Uint32 TimeManager::get_delta() const
{
	return d_time_;
}

Uint32 TimeManager::get_prev() const
{
	return prev_time_;
}

TimeManager::operator Uint32() const
{
	return curr_time_;
}

TimeManager::~TimeManager()
{

}