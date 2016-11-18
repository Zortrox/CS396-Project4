#pragma once

#include <Box2D.h>
#include <allegro5\allegro.h>
#include <vector>

class Physics {
public:
	Physics(int width, int height);
	~Physics();
	void step(float32 dt);
	b2Body* addGround();
	b2Body* addBox(int x, int y, int width, int height, float32 angle = 0.0f, float32 density = 1.0f, float32 friction = 1.0f);
	b2Body* addCircle(int x, int y, int radius);
	b2Body* addPlatform(int x, int y, int width, int height);
	b2Joint* addJoint(b2Body* bodyA, b2Body* bodyB);
	void destroyJoint(b2Joint* joint);
	b2Vec2 getTrajectoryPoint(b2Vec2 &startingPosition, b2Vec2 &startingVelocity, float32 n);

private:
	b2World* m_world;
	b2Vec2 m_gravity;

	float m_worldWidth;
	float m_worldHeight;

	std::vector<b2Body*> vecBodies;
};