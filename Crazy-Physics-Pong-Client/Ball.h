#ifndef H_BALL
#define H_BALL

#include "PhysicalObject.h"

namespace CPPong {

	class Ball : public PhysicalObject
	{
	private:
		const float32 radius = 10.f;
		const size_t pointsCount = 30;
		const float32 constVelocity = 0.1f;

		b2Vec2 direction = b2Vec2(1.f, 0.f);
		b2Vec2 acceleration = b2Vec2_zero;
		b2Vec2 initPos;

		void Reset();
	public:
		Ball(b2World* world, const b2Vec2& initPos);
		~Ball();

		void CheckPhysics();
		void ReflectDirection(b2Vec2 normal);
		void Boost(float factor);

		void SetAcceleration(b2Vec2 acceleration);
		void ClearAcceleration();

		void ResetToSide(bool leftScored);

		inline void SetDirection(float x, float y) { direction.x; direction.y; }
		inline void SetAcceleration(float x, float y) { acceleration.x; acceleration.y; }
	};

}

#endif