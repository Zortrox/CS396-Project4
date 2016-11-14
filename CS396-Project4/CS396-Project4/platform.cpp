#include "platform.h"

Platform::Platform(ALLEGRO_BITMAP* sprite, b2Body* body) {
	m_sprite = sprite;
	m_body = body;
	m_physType = body->GetType();

	m_body->SetUserData(this);
	m_object = OBJ_STRUCTURE;
	m_destroyed = false;
}

Platform::~Platform() {
}

void Platform::changeType(b2BodyType type) {
	m_physType = type;
}

bool Platform::setType() {
	if (m_body->GetType() != m_physType) {
		m_body->SetType(m_physType);
		return true;
	}
	return false;
}