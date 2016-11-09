#include "physics.h"

#include <allegro5\allegro_primitives.h>

Physics::Physics() {
	m_gravity.Set(0.0f, 9.8f);
	m_world = new b2World(m_gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(0, 20.5);
	groundBodyDef.angle = 0;
	m_groundBody = m_world->CreateBody(&groundBodyDef);
	b2PolygonShape groundShape;
	groundShape.SetAsBox(27, 0.0f);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.density = 1;
	m_groundBody->CreateFixture(&groundFixtureDef);

	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(10, 1);
	boxBodyDef.angle = 0;
	m_boxBody = m_world->CreateBody(&boxBodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(1, 1);
	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	m_boxBody->CreateFixture(&boxFixtureDef);
}

void Physics::step(float32 dt) {
	m_world->Step(dt, 8, 1);
}

void Physics::draw(ALLEGRO_DISPLAY* display) {
	//al_set_target_bitmap(al_get_backbuffer(display));
	b2Vec2 pos = m_boxBody->GetPosition();
	al_draw_rectangle(pos.x * 30 - 15, pos.y * 30 - 15, pos.x * 30 + 15, pos.y * 30 + 15, al_map_rgb(0, 255, 0), 2);
}