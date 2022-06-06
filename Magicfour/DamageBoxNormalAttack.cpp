#include "DamageBoxNormalAttack.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"


DamageBoxNormalAttack::DamageBoxNormalAttack(ObjectManager & parent_object_manager,
	Uint32 create_time, double x_position, Vector2d<double> v_per_s, Direction direction)
	: DamageBox(parent_object_manager, create_time, Point<double>(x_position, GROUND_Y - 20),
		Range<int>({ -20, -82 }, { 20, 2 }), 10, v_per_s, 320, 1 << 20, 2)
{
	change_direction(direction);
	v_ += 0.1 * UNIT_VECTOR2D_X * direction_weight();
	position_.x += 60 * direction_weight();
}

bool DamageBoxNormalAttack::update(const TimeManager & tm)
{
	return DamageBox::update(tm);
}

void DamageBoxNormalAttack::draw(Renderer& renderer, const TimeManager& tm)
{
	renderer.enroll_texture('O', 6, position_, keep_state_time(tm) / 40, get_flip());
}

DamageBoxNormalAttack::~DamageBoxNormalAttack()
{
}
