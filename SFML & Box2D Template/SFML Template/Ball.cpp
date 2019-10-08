#include "Ball.h"

// External
#include <SFML/Graphics/CircleShape.hpp>
#include <Box2D/Box2D.h>

// Internal
#include "Constants.hpp"

namespace CPPong {

	Ball::Ball(b2World* world, const b2Vec2& initPos) : PhysicalObject(ET_Ball)
	{
		// --- Internal ---
		this->initPos = b2Vec2(initPos.x / W2P, initPos.y / W2P);

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
		body->SetLinearDamping(0.f);

		// Shape
		b2CircleShape shape;
		shape.m_radius = radius / W2P;

		// Fixture
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.f;
		fixtureDef.shape = &shape;

		// Fixture - Contact filtering
		fixtureDef.filter.categoryBits = ET_Ball;
		fixtureDef.filter.maskBits = ET_Any;

		body->CreateFixture(&fixtureDef);

		// Set user data
		body->SetUserData(this);
		body->SetBullet(true);
	}

	Ball::~Ball()
	{
	}

	void Ball::CheckPhysics()
	{
		//b2Vec2 vel = body->GetLinearVelocity();

		//b2Vec2 velDif = b2Vec2(direction.x * constVelocity - vel.x, direction.y * constVelocity - vel.y);
		b2Vec2 velDif = b2Vec2(direction.x * constVelocity, direction.y * constVelocity);

		b2Vec2 impulseForce = b2Vec2(velDif.x * body->GetMass(), velDif.y * body->GetMass());

		b2Vec2 acc = b2Vec2(acceleration.x * body->GetMass(), acceleration.y * body->GetMass());

		body->ApplyLinearImpulseToCenter(impulseForce, true);
		body->ApplyLinearImpulseToCenter(acc, true);
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

	void Ball::SetAcceleration(b2Vec2 acceleration)
	{
		this->acceleration = acceleration;
	}

	void Ball::ClearAcceleration()
	{
		acceleration = b2Vec2_zero;
	}

	void Ball::Reset()
	{
		body->SetAngularVelocity(0.f);
		body->SetLinearVelocity(b2Vec2_zero);
		body->SetTransform(initPos, 0.f);

		// Setting ball to move horizontally
		// OBS: Doesn't need to flip direction manually because it was already done in the collision.
		direction.y = 0.f;
		direction.Normalize();

		acceleration = b2Vec2_zero;
	}

}