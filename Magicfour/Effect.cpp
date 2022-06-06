#include "Effect.h"
#include "Renderer.h"
#include "TimeManager.h"

Effect::Effect(ObjectManager& parent_object_manager,
	char alpha, int id, Uint32 create_time, Uint32 frame_time,
	Point<double> position, SDL_RendererFlip flip, Vector2d<double> v_per_s, Uint32 life_time)
	: AutoDeleteFieldObject(parent_object_manager, create_time, life_time, position,
		{ {0, 0}, {0, 0} }, v_per_s), alpha_(alpha), id_(id), frame_time_(frame_time), flip_(flip)
{
	
}

bool Effect::update(const TimeManager& tm)
{
	return AutoDeleteFieldObject::update(tm);
}


void Effect::draw(Renderer& renderer, const TimeManager& tm)
{
	renderer.enroll_texture(alpha_, id_, range_, keep_state_time(tm) / frame_time_, flip_);
}

Effect::~Effect()
{
}
