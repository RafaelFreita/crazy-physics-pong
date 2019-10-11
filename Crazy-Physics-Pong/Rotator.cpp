#include "Rotator.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	Rotator::Rotator(b2World* world, const b2Vec2& initPos, float motorSpeed) : PhysicalObject(ET_Rotator)
	{
		// --- Render ---
		shape = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
		shape->setOrigin(size.x / 2.f, size.y / 2.f);
		shape->setFillColor(sf::Color::Green);

		// --- Physics ---
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;	// Prevent from sleeping - can't apply forces manually
		bodyDef.bullet = true;

		// Body
		body = world->CreateBody(&bodyDef);
		body->SetTransform(b2Vec2(initPos.x / W2P, initPos.y / W2P), float32(0));
		body->SetGravityScale(0.0f);

		// Shape
		b2PolygonShape shape;
		shape.SetAsBox((size.x / 2.f) / W2P, (size.y / 2.f) / W2P);

		// FixtureDef
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;
		fixtureDef.shape = &shape;

		// FixtureDef - Contact filtering
		fixtureDef.filter.categoryBits = ET_Rotator;
		fixtureDef.filter.maskBits = ET_Any;

		body->CreateFixture(&fixtureDef);

		// Set user data
		body->SetUserData(this);
		
		// JOINT
		// Dummy body
		b2BodyDef dummyBodyDef;
		dummyBodyDef.type = b2_staticBody;
		dummyBodyDef.allowSleep = false;
		dummyBodyDef.gravityScale = 0.f;

		b2Body* dummyBody = world->CreateBody(&dummyBodyDef);
		dummyBody->SetTransform(b2Vec2(initPos.x / W2P, initPos.y / W2P), float32(0));

		auto revoluteJointDef = b2RevoluteJointDef();
		revoluteJointDef.bodyA = body;
		revoluteJointDef.bodyB = dummyBody;
		revoluteJointDef.enableMotor = true;
		revoluteJointDef.motorSpeed = motorSpeed;
		revoluteJointDef.maxMotorTorque = 1000.f;
		auto revoluteJoint = world->CreateJoint(&revoluteJointDef);
	}

	Rotator::~Rotator()
	{
	}

	void Rotator::Destroy(b2World* world)
	{
		world->DestroyBody(body);
	}

	void Rotator::CheckPhysics()
	{
	}

}