#include "Player.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	std::map<PlayerType, PlayerSpecs> const Player::playerTypeMap = {
		{PlayerType::Default, {1.f, .5f}},
		{PlayerType::Wood, {1.3f, .66f}},
		{PlayerType::Rubber, {1.6f, .33f}},
		{PlayerType::Velcro, {1.f, 1.f}}
	};

	Player::Player(b2World* world, const b2Vec2& initPos) : PhysicalObject(ET_Player)
	{
		// --- Internal ---
		this->initPos = b2Vec2(initPos.x / W2P, initPos.y / W2P);

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
		fixtureDef.restitution = 1.0f;
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
		axisLockedPosition.x = initPos.x;

		body->SetTransform(axisLockedPosition, lockedAngle);
	}

	void Player::SetType(PlayerType playerType)
	{
		PlayerSpecs playerSpecs = playerTypeMap.at(playerType);
		ballRicochetFactor = playerSpecs.ballRicochetFactor;
		ballEffectFactor = playerSpecs.ballEffectFactor;
	}

	void Player::Reset()
	{
		body->SetLinearVelocity(b2Vec2_zero);
		body->SetTransform(initPos, 0.f);
	}

}