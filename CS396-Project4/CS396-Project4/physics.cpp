#include "globals.h"
#include "physics.h"
#include "contact_listener.h"
#include <allegro5\allegro_primitives.h>

Physics::Physics(int width, int height) {
	m_gravity.Set(0.0f, 9.8f);
	m_world = new b2World(m_gravity);
	m_world->SetContactListener(new ContactListener());

	m_worldWidth = 1.0f * width / PHYS_PIX;
	m_worldHeight = 1.0f * height / PHYS_PIX;
}
Physics::~Physics() {

}

void Physics::step(float32 dt) {
	m_world->Step(dt, 8, 1);
}

b2Body* Physics::addGround() {
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(m_worldWidth / 2, m_worldHeight + 1.0f);
	groundBodyDef.angle = 0;
	vecBodies.push_back(m_world->CreateBody(&groundBodyDef));
	b2Body* groundBody = vecBodies.back();

	b2PolygonShape groundShape;
	groundShape.SetAsBox(m_worldWidth / 2, 1.0f);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.density = 1;
	groundBody->CreateFixture(&groundFixtureDef);

	return groundBody;
}

b2Body* Physics::addBox(int x, int y, int width, int height, float32 density, float32 friction) {
	//remove small buffer area from bitmaps
	width -= 1;
	height -= 1;

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
	boxFixtureDef.density = density;
	boxFixtureDef.friction = friction;
	boxBody->CreateFixture(&boxFixtureDef);

	return boxBody;
}

b2Body* Physics::addCircle(int x, int y, int radius) {
	b2BodyDef circleBodyDef;
	circleBodyDef.type = b2_dynamicBody;
	circleBodyDef.position.Set(1.0f * x / PHYS_PIX, 1.0f * y / PHYS_PIX);
	circleBodyDef.angle = 0;
	circleBodyDef.bullet = true;
	vecBodies.push_back(m_world->CreateBody(&circleBodyDef));
	b2Body* boxBody = vecBodies.back();

	b2FixtureDef circleFixtureDef;
	b2CircleShape circleShape;
	circleShape.m_radius = 1.0f * radius / PHYS_PIX;
	circleFixtureDef.shape = &circleShape;
	circleFixtureDef.density = 10;
	boxBody->CreateFixture(&circleFixtureDef);

	return boxBody;
}

b2Body* Physics::addPlatform(int x, int y, int width, int height) {
	b2BodyDef movingBodyDef;
	movingBodyDef.type = b2_kinematicBody;
	movingBodyDef.position.Set(x / PHYS_PIX, y / PHYS_PIX);
	movingBodyDef.angle = 0;

	vecBodies.push_back(m_world->CreateBody(&movingBodyDef));
	b2Body* movingBody = vecBodies.back();

	b2PolygonShape movingShape;
	movingShape.SetAsBox(1.0f * width / (PHYS_PIX * 2), 1.0f * height / (PHYS_PIX * 2));
	b2FixtureDef movingFixtureDef;
	movingFixtureDef.shape = &movingShape;
	movingFixtureDef.density = 1;
	movingBody->CreateFixture(&movingFixtureDef);

	return movingBody;
}

b2Joint * Physics::addJoint(b2Body * bodyA, b2Body * bodyB) {
	b2WeldJointDef jointDef;
	jointDef.bodyA = bodyA;
	jointDef.bodyB = bodyB;
	jointDef.collideConnected = true;

	return m_world->CreateJoint(&jointDef);
}

void Physics::destroyJoint(b2Joint * joint) {
	m_world->DestroyJoint(joint);
}

b2Vec2 Physics::getTrajectoryPoint(b2Vec2 &startingPosition, b2Vec2 &startingVelocity, float32 n)
{
	float32 t = 1 / 60.0f;
	b2Vec2 stepVelocity = t * startingVelocity;
	b2Vec2 stepGravity = t * t * m_world->GetGravity();

	return startingPosition + n * stepVelocity + 0.5f * (n * n + n) * stepGravity;
}