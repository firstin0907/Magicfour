#pragma once
#include "header.h"

class TimeManager
{
private:
	Uint32			curr_time_;		// ���� �������� ���� �ð�
	Uint32			prev_time_;		// ���� �������� curr_time_
	Uint32			d_time_;		// curr_time - prev_time

public:
	TimeManager();
	
	// curr_time�� ������Ʈ���ش�.
	void update();

	Uint32 get_curr() const;
	Uint32 get_delta() const;
	Uint32 get_prev() const;
	
	operator Uint32() const;

	~TimeManager();
};

