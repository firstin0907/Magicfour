#pragma once
#include "Renderer.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "TimeManager.h"

// 매개 변수 하나로 각종 ~~Manager들을 넘겨주기 위한 구조체
struct Managers
{
	Renderer&			rdr;
	ObjectManager&		om;
	SoundManager&		sm;
	TimeManager&		tm;

public:
	Managers(Renderer& rdr, ObjectManager& om, SoundManager& sm, TimeManager& tm);
	~Managers();
};

