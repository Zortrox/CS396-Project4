#pragma once

#include <allegro5\allegro.h>
#include <Box2D\Box2D.h>

enum object {OBJ_BALL, OBJ_STRUCTURE, OBJ_PERSON};

class PhysObject {
public:
	PhysObject(ALLEGRO_BITMAP* sprite, b2Body* body, int object);
	~PhysObject();

	void draw(ALLEGRO_DISPLAY* display);
	void fire(b2Vec2 velocity);
	virtual void destroy();
	bool getDestroyed();
	void setObject(int object);
	int getObject();

	b2Body* getBody();

	void setPoints(bool points);
	virtual int calcPoints();

protected:
	PhysObject();

	ALLEGRO_BITMAP* m_sprite;
	b2Body* m_body;
	int m_object;
	bool m_destroyed;
	bool m_addPoints;
};