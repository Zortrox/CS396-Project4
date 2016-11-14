#pragma once

#include "phys_object.h"

class Platform : public PhysObject {
public:
	Platform(ALLEGRO_BITMAP* sprite, b2Body* body);
	~Platform();

	void changeType(b2BodyType type);
	bool setType();

private:
	b2BodyType m_physType;
};