#ifndef H_PLAYER
#define H_PLAYER

#include "PhysicalObject.h"

namespace CPPong {

	class Player : public PhysicalObject
	{
	private:
		const b2Vec2	size			= { 16.f, 64.f };
		const float32	movementForce	= 3.f;
		float32			xPosition;
	public:
		Player(b2World* world, const b2Vec2& initPos);
		~Player();

		inline void MoveUp()	{ body->ApplyLinearImpulseToCenter(b2Vec2(5.f, -movementForce), false); }
		inline void MoveDown()	{ body->ApplyLinearImpulseToCenter(b2Vec2(0.f, +movementForce), false); }

		// Locking player to Y axis.
		void CheckPhysics();
	};

}

#endif