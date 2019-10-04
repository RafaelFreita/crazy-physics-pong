#include "Ball.h"

// External
#include <SFML/Graphics/CircleShape.hpp>
#include <Box2D/Box2D.h>

// Internal
#include "Constants.hpp"

namespace CPPong {

	Ball::Ball(b2World* world, const b2Vec2& initPos) : PhysicalObject(T_Ball)
	{
		// --- Render ---
		shape = new sf::CircleShape(radius);
		shape->setOrigin(radius, radius);

		// Color
		shape->setFillColor(sf::Color::Red);

		// --- Physics ---
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;	// Prevent from sleeping - can't apply forces manually

		// Body
		body = world->CreateBody(&bodyDef);
		body->SetTransform(b2Vec2(initPos.x / W2P, initPos.y / W2P), float32(0));
		body->SetGravityScale(0.0f);
		body->SetLinearDamping(1.f);

		// Shape
		b2CircleShape shape;
		shape.m_radius = radius / W2P;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.3f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;
		fixtureDef.shape = &shape;

		// Fixture - Contact filtering
		fixtureDef.filter.categoryBits = T_Ball;
		fixtureDef.filter.maskBits = T_Player;

		body->CreateFixture(&fixtureDef);
	}

	Ball::~Ball()
	{
	}

	void Ball::CheckPhysics()
	{
	}

}