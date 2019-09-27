#ifndef H_PHYSICSOBJECT
#define H_PHYSICSOBJECT

#include <Box2D/Dynamics/b2Body.h>

#include "GameObject.h"

namespace CPPong {

	class PhysicalObject : public GameObject
	{
	public:
		PhysicalObject();
		~PhysicalObject();

		PhysicalObject(sf::Shape* shape, b2Body* body);

		b2Body* body;
	};

}

#endif