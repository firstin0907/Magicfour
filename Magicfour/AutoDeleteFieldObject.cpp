#include "AutoDeleteFieldObject.h"
#include "global.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"



AutoDeleteFieldObject::AutoDeleteFieldObject(ObjectManager& parent_object_manager, Uint32 create_time,
	Uint32 life_time, Point<double> position, Range<int> range, Vector2d<double> v_per_s)
	: FieldObject(parent_object_manager, create_time, position, range, v_per_s),
	expiry_time_(create_time + life_time)
{
	if (life_time == ETERNAL) expiry_time_ = ETERNAL;
}

bool AutoDeleteFieldObject::update(const TimeManager& tm)
{
	return tm.get_curr() < expiry_time_;
}


AutoDeleteFieldObject::~AutoDeleteFieldObject()
{
}
