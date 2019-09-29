#ifndef H_PLAYER
#define H_PLAYER

#include "PhysicalObject.h"
#include <iostream>

namespace CPPong {

	class Player
	{
	private:
		const b2Vec2	size			= { 16.f, 64.f };
		const float32	movementForce	= float32(10.f);

		PhysicalObject* physical;
	public:
		Player(b2World* world, const b2Vec2& initPos);
		~Player();

		inline PhysicalObject* GetPhysicalObj() { return physical; }
		void MoveUp();
		void MoveDown();
	};

}

#endif