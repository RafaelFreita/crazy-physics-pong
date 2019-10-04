#ifndef H_BALL
#define H_BALL

#include "PhysicalObject.h"

namespace CPPong {

	class Ball : public PhysicalObject
	{
	private:
		const float32 radius = 5.f;
		const size_t pointsCount = 30;
	public:
		Ball(b2World* world, const b2Vec2& initPos);
		~Ball();

		void CheckPhysics();
	};

}

#endif