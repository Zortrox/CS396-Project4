#include "person.h"

Person::Person(ALLEGRO_BITMAP* sprite, b2Body* body) {
	m_sprite = sprite;
	m_body = body;
	m_object = OBJ_PERSON;

	m_body->SetUserData(this);
	m_destroyed = false;
	m_addPoints = false;
}

Person::~Person() {
}

void Person::destroy() {
	if (!m_destroyed) {
		m_destroyed = true;
		m_addPoints = true;
	}
}

int Person::calcPoints() {
	if (m_addPoints) {
		m_addPoints = false;
		return 100;
	}
	return 0;
}
