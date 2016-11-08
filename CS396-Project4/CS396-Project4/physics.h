#pragma once

#include <Box2D.h>
#include <allegro5\allegro.h>

class Physics {
public:
	Physics();
	void step(float32 dt);
	void draw(ALLEGRO_DISPLAY* display);

private:
	b2World* m_world;
	b2Body* m_groundBody;
	b2Body* m_boxBody;
	b2Vec2 m_gravity;
};