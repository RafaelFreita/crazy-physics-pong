#include "ContactListener.h"

// Internal
#include "PhysicalObject.h"
#include "Player.h"
#include "Ball.h"

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

		Ball* ball = NULL;
		if (typeA == ET_Ball) {
			ball = static_cast<Ball*>(userDataA);
		}
		else if (typeB == ET_Ball) {
			ball = static_cast<Ball*>(userDataB);
		}

		// Check if there is a player
		Player* player = NULL;
		if (typeA == ET_Player) {
			player = static_cast<Player*>(userDataA);
		}
		else if (typeB == ET_Player) {
			player = static_cast<Player*>(userDataB);
		}

		// Apply extra force based on player movement?
		if (player) {}

		// Change ball direction
		if (ball) {
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			ball->ReflectDirection(worldManifold.normal);
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