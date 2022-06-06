#pragma once
#include "header.h"

class TimeManager
{
private:
	Uint32			curr_time_;		// 현재 프레임의 기준 시간
	Uint32			prev_time_;		// 이전 프레임의 curr_time_
	Uint32			d_time_;		// curr_time - prev_time

public:
	TimeManager();
	
	// curr_time을 업데이트해준다.
	void update();

	Uint32 get_curr() const;
	Uint32 get_delta() const;
	Uint32 get_prev() const;
	
	operator Uint32() const;

	~TimeManager();
};

