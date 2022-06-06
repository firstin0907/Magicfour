#include "FontEffect.h"

#include "Renderer.h"
#include "ObjectManager.h"
#include "TimeManager.h"
#include "SoundManager.h"


FontEffect::FontEffect(ObjectManager& parent_object_manager, Uint32 create_time,
	Renderer& renderer, Point<double> position, int font_id, SDL_Color color, std::string content,
	Vector2d<double> v_per_s, Uint32 life_time, FontEffect::OPTION option)
	: AutoDeleteFieldObject(parent_object_manager, create_time, life_time, position, { {0, 0}, {0, 0} }, v_per_s)
{
	texture_ = renderer.make_text(content, font_id, color);
	if (option & ALIGN_CENTER)
	{
		texture_->center.x += texture_->size.x / 2;
	}
	else if (option & ALIGN_RIGHT)
	{
		texture_->center.x += texture_->size.x;
	}

}

bool FontEffect::update(const TimeManager& tm)
{
	return AutoDeleteFieldObject::update(tm);
}

void FontEffect::draw(Renderer& renderer, const TimeManager& tm)
{
	renderer.enroll_texture(texture_, position_, 0, SDL_FLIP_NONE);
}

FontEffect::~FontEffect()
{
}
