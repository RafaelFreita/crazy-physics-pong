#include "Player.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <Box2D/Box2D.h>

#include "Constants.hpp"

namespace CPPong {

	Player::Player(b2World* world, const b2Vec2& initPos)
	{
		// --- Render ---
		static int i = 0;
		sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(size.x, size.y));
		rectangle->setOrigin(size.x / 2.f, size.y / 2.f);

		// Color
		if (i == 0) {
			rectangle->setFillColor(sf::Color::White);
			i++;
		}
		else {
			rectangle->setFillColor(sf::Color::Green);
		}

		// --- Physics ---
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = false;	// Prevent player from sleeping - can't apply forces manually

		b2Body* body = world->CreateBody(&bodyDef);
		body->SetTransform(b2Vec2(initPos.x / W2P, initPos.y / W2P), float32(0));
		body->SetGravityScale(0.0f);
		body->SetLinearDamping(5.f);

		b2PolygonShape shape;
		shape.SetAsBox((32.f / 2.f) / W2P, (32.f / 2.f) / W2P);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.5f;
		fixtureDef.friction = 0.5f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);

		physical = new PhysicalObject(rectangle, body);
	}

	Player::~Player()
	{
	}

}