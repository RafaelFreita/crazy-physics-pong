#include "Box.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	Box::Box(b2World* world, const b2Vec2& initPos, sf::Vector2f size) : PhysicalObject(ET_Any)
	{
		// --- Render ---
		shape = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
		shape->setOrigin(size.x / 2.f, size.y / 2.f);
		shape->setFillColor(sf::Color::White);

		// --- Physics ---
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;

		// Body
		b2Body* body = world->CreateBody(&bodyDef);
		body->SetTransform(initPos, float32(0));
		body->SetGravityScale(1.0f);

		// Shape
		b2PolygonShape shape;
		shape.SetAsBox((size.x / 2.f) / W2P, (size.y / 2.f) / W2P);

		// FixtureDef
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.5f;
		fixtureDef.restitution = 0.5f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}


	Box::~Box()
	{
	}

	void Box::CheckPhysics()
	{
	}

}