#include "phys_object.h"
#include "globals.h"

PhysObject::PhysObject(ALLEGRO_BITMAP* sprite, b2Body* body) {
	m_sprite = sprite;
	m_body = body;
}

PhysObject::~PhysObject() {
}

void PhysObject::draw(ALLEGRO_DISPLAY * display) {
	al_set_target_bitmap(al_get_backbuffer(display));

	float32 ang = m_body->GetAngle();
	b2Vec2 pos = m_body->GetPosition();
	int width = al_get_bitmap_width(m_sprite);

	al_draw_rotated_bitmap(m_sprite, width / 2, width / 2, pos.x * PHYS_PIX, pos.y * PHYS_PIX, ang, false);
}

void PhysObject::fire(b2Vec2 impulse)
{
	m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);
}
