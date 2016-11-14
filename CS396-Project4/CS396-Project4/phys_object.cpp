#include "phys_object.h"
#include "globals.h"
#include <allegro5\allegro_primitives.h>

PhysObject::PhysObject(ALLEGRO_BITMAP* sprite, b2Body* body, int object) {
	m_sprite = sprite;
	m_body = body;
	m_object = object;

	m_body->SetUserData(this);
	m_destroyed = false;
	m_addPoints = false;
}

PhysObject::PhysObject() {

}

PhysObject::~PhysObject() {
}

void PhysObject::draw(ALLEGRO_DISPLAY * display) {
	al_set_target_bitmap(al_get_backbuffer(display));

	float32 ang = m_body->GetAngle();
	b2Vec2 pos = m_body->GetPosition();
	int width = al_get_bitmap_width(m_sprite);
	int height = al_get_bitmap_height(m_sprite);

	al_draw_rotated_bitmap(m_sprite, width / 2, height / 2, pos.x * PHYS_PIX, pos.y * PHYS_PIX, ang, false);
	if (m_destroyed) {
		al_draw_filled_circle(pos.x * PHYS_PIX, pos.y * PHYS_PIX, width / 4, al_map_rgb(255, 0, 0));
	}
}

void PhysObject::fire(b2Vec2 velocity)
{
	m_body->SetLinearVelocity(velocity);
}

void PhysObject::destroy() {
	m_destroyed = true;
}

bool PhysObject::getDestroyed() {
	return m_destroyed;
}

void PhysObject::setObject(int object) {
	m_object = object;
}

int PhysObject::getObject() {
	return m_object;
}

b2Body* PhysObject::getBody() {
	return m_body;
}

void PhysObject::setPoints(bool points) {
	m_addPoints = points;
}

int PhysObject::calcPoints() {
	return 0;
}
