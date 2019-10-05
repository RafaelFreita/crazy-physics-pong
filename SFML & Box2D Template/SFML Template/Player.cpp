#include "Player.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	Player::Player(b2World* world, const b2Vec2& initPos) : PhysicalObject(ET_Player)
	{
		// --- Internal ---
		xPosition = initPos.x / W2P;

		// --- Render ---
		static int i = 0;
		shape = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
		shape->setOrigin(size.x / 2.f, size.y / 2.f);

		// Color
		if (i == 0) {
			shape->setFillColor(sf::Color::White);
			i++;
		}
		else {
			shape->setFillColor(sf::Color::Green);
		}

		// --- Physics ---
		// BodyDef
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;	// Prevent from sleeping - can't apply forces manually

		// Body
		body = world->CreateBody(&bodyDef);
		body->SetTransform(b2Vec2(initPos.x / W2P, initPos.y / W2P), float32(0));
		body->SetGravityScale(0.0f);
		body->SetLinearDamping(5.f);
		body->SetFixedRotation(true); // Fix player rotation

		// Shape
		b2PolygonShape shape;
		shape.SetAsBox((size.x / 2.f) / W2P, (size.y / 2.f) / W2P);

		// FixtureDef
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 0.5f;
		fixtureDef.shape = &shape;

		// FixtureDef - Contact filtering
		fixtureDef.filter.categoryBits = ET_Player;
		fixtureDef.filter.maskBits = ET_Ball | ET_Wall;

		body->CreateFixture(&fixtureDef);

		// Set user data
		body->SetUserData(this);
	}

	Player::~Player()
	{
	}

	void Player::CheckPhysics()
	{
		const static float32 lockedAngle = float32(0.f);
		
		b2Vec2 axisLockedPosition = body->GetPosition();
		axisLockedPosition.x = xPosition;

		body->SetTransform(axisLockedPosition, lockedAngle);
	}

}