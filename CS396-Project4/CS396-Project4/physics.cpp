#include "physics.h"
#include "globals.h"
#include <allegro5\allegro_primitives.h>

Physics::Physics() {
	m_gravity.Set(0.0f, 9.8f);
	m_world = new b2World(m_gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 19.5);
	groundBodyDef.angle = 0;
	m_groundBody = m_world->CreateBody(&groundBodyDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(27, 0.0f);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.density = 1;
	m_groundBody->CreateFixture(&groundFixtureDef);
}
Physics::~Physics() {

}

void Physics::step(float32 dt) {
	m_world->Step(dt, 8, 1);
}

b2Body* Physics::addBox(int x, int y, int width, int height = 0) {
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(1.0f * x / PHYS_PIX, 1.0f * y / PHYS_PIX);
	boxBodyDef.angle = 0;
	vecBodies.push_back(m_world->CreateBody(&boxBodyDef));
	b2Body* boxBody = vecBodies.back();

	b2FixtureDef boxFixtureDef;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1.0f * width / (PHYS_PIX * 2), 1.0f * height / (PHYS_PIX * 2));
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	boxBody->CreateFixture(&boxFixtureDef);

	return boxBody;
}

b2Body* Physics::addCircle(int x, int y, int radius) {
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(1.0f * x / PHYS_PIX, 1.0f * y / PHYS_PIX);
	boxBodyDef.angle = 0;
	vecBodies.push_back(m_world->CreateBody(&boxBodyDef));
	b2Body* boxBody = vecBodies.back();

	b2FixtureDef boxFixtureDef;
	b2CircleShape circleShape;
	circleShape.m_radius = 1.0f * radius / PHYS_PIX;
	boxFixtureDef.shape = &circleShape;
	boxFixtureDef.density = 1;
	boxBody->CreateFixture(&boxFixtureDef);

	return boxBody;
}

b2Vec2 Physics::getTrajectoryPoint(b2Vec2 &startingPosition, b2Vec2 &startingVelocity, float32 n)
{
	float32 t = 1 / 60.0f;
	b2Vec2 stepVelocity = t * startingVelocity;
	b2Vec2 stepGravity = t * t * m_world->GetGravity();

	return startingPosition + n * stepVelocity + 0.5f * (n * n + n) * stepGravity;
}