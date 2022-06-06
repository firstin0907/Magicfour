#include "DamageBoxLeg.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"


DamageBoxLeg::DamageBoxLeg(ObjectManager& parent_object_manager, Uint32 create_time, double x_position)
	: DamageBox(parent_object_manager, create_time, Point<double>(x_position, GROUND_Y),
		Range<int>({ -29, 0 }, { 29, 30 } ), 10, { 0.0, -176.0 * 4 }, 250U, 1 << 30, 0.7)
{

}

bool DamageBoxLeg::update(const TimeManager& tm)
{
	if (DamageBox::update(tm) == false) return false;

	range_.p2.y = GROUND_Y - static_cast<int>(position_.y);
	
	return true;
}

void DamageBoxLeg::draw(Renderer& renderer, const TimeManager& tm)
{
	int present_h = GROUND_Y - static_cast<int>(position_.y);

	renderer.draw_range(range_ + position_);

	renderer.enroll_texture('O', 3, Range<int>{ {0, 0}, { 58, present_h } }, static_cast<Range<int> >(range_ + position_));
}

DamageBoxLeg::~DamageBoxLeg()
{
}
