#include "contact_listener.h"
#include "phys_object.h"
#include "platform.h"
#include "person.h"

void ContactListener::BeginContact(b2Contact * contact) {
	b2BodyType typeA = contact->GetFixtureA()->GetBody()->GetType();
	b2BodyType typeB = contact->GetFixtureB()->GetBody()->GetType();

	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	PhysObject* objA = static_cast<PhysObject*>(bodyA->GetUserData());
	PhysObject* objB = static_cast<PhysObject*>(bodyB->GetUserData());

	if (typeA == b2_kinematicBody && objB->getObject() == OBJ_BALL) {
		static_cast<Platform*>(objA)->changeType(b2_dynamicBody);
		contact->SetEnabled(false);
	}
	else if (typeB == b2_kinematicBody && objA->getObject() == OBJ_BALL) {
		static_cast<Platform*>(objB)->changeType(b2_dynamicBody);
		contact->SetEnabled(false);
	}

	if (objA->getObject() == OBJ_PERSON) {
		float32 force = (bodyA->GetLinearVelocity() - bodyB->GetLinearVelocity()).Length() * (bodyA->GetMass() + bodyB->GetMass());
		if (force > 2.5) {
			objA->destroy();
		}
	}
	if (objB->getObject() == OBJ_PERSON) {
		float32 force = (bodyA->GetLinearVelocity() - bodyB->GetLinearVelocity()).Length() * (bodyA->GetMass() + bodyB->GetMass());
		if (force > 2.5) {
			objB->destroy();
		}
	}
}

void ContactListener::EndContact(b2Contact * contact) {
}
