#include "contact_listener.h"
#include "phys_object.h"

void ContactListener::BeginContact(b2Contact * contact) {
	if (contact->GetFixtureA()->GetBody()->GetType() == b2_kinematicBody) {
		static_cast<PhysObject*>(contact->GetFixtureA()->GetBody()->GetUserData())->changeType(b2_dynamicBody);
		contact->SetEnabled(false);
	}
	else if (contact->GetFixtureB()->GetBody()->GetType() == b2_kinematicBody) {
		static_cast<PhysObject*>(contact->GetFixtureB()->GetBody()->GetUserData())->changeType(b2_dynamicBody);
		contact->SetEnabled(false);
	}
}

void ContactListener::EndContact(b2Contact * contact) {
}
