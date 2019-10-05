#ifndef H_BALL
#define H_BALL

#include "PhysicalObject.h"

namespace CPPong {

	class Ball : public PhysicalObject
	{
	private:
		const float32 radius = 10.f;
		const size_t pointsCount = 30;
		const float32 constVelocity = 10.f;

		b2Vec2 direction = b2Vec2(1.f, 0.f);
	public:
		Ball(b2World* world, const b2Vec2& initPos);
		~Ball();

		void CheckPhysics();
		void ChangeDirection(b2Vec2 direction);
		void ReflectDirection(b2Vec2 normal);
	};

}

#endif