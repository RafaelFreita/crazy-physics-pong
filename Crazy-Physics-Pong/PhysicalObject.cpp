#include "PhysicalObject.h"

namespace CPPong {

	PhysicalObject::PhysicalObject(EntityType type) : type(type)
	{
	}

	PhysicalObject::PhysicalObject(sf::Shape* shape, b2Body* body, EntityType type) : GameObject(shape), body(body), type(type)
	{
	}

	PhysicalObject::~PhysicalObject()
	{
		// TODO: Clear pointer? Box2D Caveats
		// delete body;
	}

}