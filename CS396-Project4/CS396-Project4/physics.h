#pragma once

#include <Box2D.h>
#include <allegro5\allegro.h>
#include <vector>

class Physics {
public:
	Physics();
	~Physics();
	void step(float32 dt);
	b2Body* addBox(int x, int y, int width, int height);
	b2Body* addCircle(int x, int y, int radius);
	b2Body* addPlatform(int x, int y, int width, int height);
	b2Vec2 getTrajectoryPoint(b2Vec2 &startingPosition, b2Vec2 &startingVelocity, float32 n);

private:
	b2World* m_world;
	b2Vec2 m_gravity;

	b2Body* m_groundBody;

	std::vector<b2Body*> vecBodies;
};