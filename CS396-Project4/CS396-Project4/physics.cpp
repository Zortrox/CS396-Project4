#include "physics.h"
#include "globals.h"
#include <allegro5\allegro_primitives.h>

Physics::Physics() {
	m_gravity.Set(0.0f, 9.8f);
	m_world = new b2World(m_gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 20);
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

b2Body* Physics::addBody(float x, float y) {
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(x, y);
	boxBodyDef.angle = 0;
	vecBodies.push_back(m_world->CreateBody(&boxBodyDef));
	b2Body* boxBody = vecBodies.back();
	b2PolygonShape boxShape;
	boxShape.SetAsBox(.5, .5);
	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	boxBody->CreateFixture(&boxFixtureDef);

	return boxBody;
}