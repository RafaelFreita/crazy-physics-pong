#include "Goal.h"

#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	Goal::Goal(b2World* world, const b2Vec2& p1, const b2Vec2& p2, bool isLeft) : PhysicalObject(ET_Goal), isLeft(isLeft)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.allowSleep = true;
		bodyDef.position.SetZero();

		// Body
		body = world->CreateBody(&bodyDef);

		// Shape
		b2EdgeShape shape;
		shape.Set(p1, p2);

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.friction = 0.f;
		fixtureDef.restitution = 0.f;
		fixtureDef.shape = &shape;

		// Fixture - Contact filtering
		fixtureDef.filter.categoryBits = ET_Goal;
		fixtureDef.filter.maskBits = ET_Any;

		body->CreateFixture(&fixtureDef);

		// Set user data
		body->SetUserData(this);
	}

	Goal::~Goal()
	{
	}

	void Goal::CheckPhysics()
	{
	}

}