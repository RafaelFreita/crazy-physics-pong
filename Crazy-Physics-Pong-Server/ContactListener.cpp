#include "ContactListener.h"

// Internal
#include "PhysicalObject.h"
#include "Player.h"
#include "Ball.h"
#include "Goal.h"

namespace CPPong {

	void ContactListener::BeginContact(b2Contact* contact)
	{
		EntityType typeA = ET_Any, typeB = ET_Any;

		// Get user data A type
		void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		if (userDataA) { typeA = static_cast<PhysicalObject*>(userDataA)->GetType(); }

		// Get user data B type
		void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		if (userDataB) { typeB = static_cast<PhysicalObject*>(userDataB)->GetType(); }

		
		// CHECKING TYPE - BALL
		Ball* ball = NULL;
		if (typeA == ET_Ball) { ball = static_cast<Ball*>(userDataA); }
		else if (typeB == ET_Ball) { ball = static_cast<Ball*>(userDataB); }

		// CHECKING TYPE -  PLAYER
		Player* player = NULL;
		if (typeA == ET_Player) { player = static_cast<Player*>(userDataA); }
		else if (typeB == ET_Player) { player = static_cast<Player*>(userDataB); }

		// CHECKING TYPE -  GOAL
		Goal* goal = NULL;
		if (typeA == ET_Goal) { goal = static_cast<Goal*>(userDataA); }
		else if (typeB == ET_Goal) { goal = static_cast<Goal*>(userDataB); }

		// Check if collided with goal
		if (goal && ball) {
			goal->Score();
		}

		// Change ball direction
		if (ball) {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			ball->ReflectDirection(worldManifold.normal);

			ball->ClearAcceleration(); // Always clear the acceleration when a collision happen

			// Apply accelaration to ball when colliding with player
			if (player) {
				// Create acceleration based on player movement
				// Acceleration is create to the opposite side of velocity
				b2Vec2 playerVel = player->GetVel();
				b2Vec2 impulse = b2Vec2(0.f, -playerVel.y / 100.f);
				// Multiply impulse by effect factor
				impulse *= player->GetBallEffectFactor();

				ball->SetAcceleration(impulse);

				// Apply impulse based on player ricochet factor
				ball->Boost(player->GetBallRicochetFactor());
			}
		}

	}

	void ContactListener::EndContact(b2Contact* contact)
	{
	}

	ContactListener::ContactListener()
	{
	}

	ContactListener::~ContactListener()
	{
	}

}