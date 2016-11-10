#pragma once

#include <allegro5\allegro.h>
#include <Box2D\Box2D.h>

class PhysObject {
public:
	PhysObject(ALLEGRO_BITMAP* sprite, b2Body* body);
	~PhysObject();

	void draw(ALLEGRO_DISPLAY* display);
	void fire(b2Vec2 velocity);
	void changeType(b2BodyType type);
	void setType();

private:
	ALLEGRO_BITMAP* m_sprite;
	b2Body* m_body;
	b2BodyType m_physType;
};