#ifndef H_ROTATOR
#define H_ROTATOR

#include "PhysicalObject.h"

namespace CPPong {

	class Rotator : public PhysicalObject
	{
	private:
		const b2Vec2 size = { 8.f, 128.f};
	public:
		Rotator(b2World* world, const b2Vec2& initPos, float motorSpeed);
		~Rotator();

		void Destroy(b2World* world);

		void CheckPhysics();
	};

}

#endif