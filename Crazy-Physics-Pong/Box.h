#ifndef H_BOX
#define H_BOX

#include "PhysicalObject.h"

namespace CPPong {

	class Box : public PhysicalObject
	{
	public:
		Box(b2World* world, const b2Vec2& initPos, sf::Vector2f size);
		~Box();

		void CheckPhysics();
	};

}

#endif