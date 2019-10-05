#include "Ball.h"

// External
#include <SFML/Graphics/CircleShape.hpp>
#include <Box2D/Box2D.h>

// Internal
#include "Constants.hpp"

namespace CPPong {

	Ball::Ball(b2World* world, const b2Vec2& initPos) : PhysicalObject(ET_Ball)
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
		body->SetLinearDamping(0.5f);

		// Shape
		b2CircleShape shape;
		shape.m_radius = radius / W2P;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;
		fixtureDef.shape = &shape;

		// Fixture - Contact filtering
		fixtureDef.filter.categoryBits = ET_Ball;
		fixtureDef.filter.maskBits = ET_Any;

		body->CreateFixture(&fixtureDef);

		// Set user data
		body->SetUserData(this);
	}

	Ball::~Ball()
	{
	}

	void Ball::CheckPhysics()
	{
		b2Vec2 vel = body->GetLinearVelocity();

		b2Vec2 velDif = b2Vec2(direction.x * constVelocity - vel.x, direction.y * constVelocity - vel.y);

		b2Vec2 impulseForce = b2Vec2(velDif.x * body->GetMass(), velDif.y * body->GetMass());

		body->ApplyLinearImpulseToCenter(impulseForce, false);
	}

	void Ball::ChangeDirection(b2Vec2 direction)
	{
		this->direction = direction;
		direction.Normalize();
	}

	b2Vec2 reflect(const b2Vec2& direction, const b2Vec2& normal) {
		return direction - 2 * (b2Dot(direction, normal)) * normal;
	}

	void Ball::ReflectDirection(b2Vec2 normal)
	{
		direction.Normalize();
		direction = reflect(direction, normal);
	}

}