#pragma once
#include "Renderer.h"
#include "ObjectManager.h"
#include "SoundManager.h"
#include "TimeManager.h"

// �Ű� ���� �ϳ��� ���� ~~Manager���� �Ѱ��ֱ� ���� ����ü
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

