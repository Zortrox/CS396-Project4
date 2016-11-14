#pragma once

#include "phys_object.h"

class Person : public PhysObject {
public:
	Person(ALLEGRO_BITMAP* sprite, b2Body* body);
	~Person();

	void destroy();
	int calcPoints();

private:

};