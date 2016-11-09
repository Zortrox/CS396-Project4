#pragma once

#include <Box2D.h>
#include <allegro5\allegro.h>
#include <vector>

class Physics {
public:
	Physics();
	~Physics();
	void step(float32 dt);
	b2Body* addBody(float x, float y);

private:
	b2World* m_world;
	b2Vec2 m_gravity;

	b2Body* m_groundBody;

	std::vector<b2Body*> vecBodies;
};