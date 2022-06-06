#include "Managers.h"

Managers::Managers(Renderer& rdr, ObjectManager& om, SoundManager& sm, TimeManager& tm)
	: rdr(rdr), om(om), sm(sm), tm(tm)
{
}

Managers::~Managers()
{
}
