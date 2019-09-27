#include "PhysicalObject.h"

namespace CPPong {

	PhysicalObject::PhysicalObject()
	{
	}


	PhysicalObject::~PhysicalObject()
	{
	}

	PhysicalObject::PhysicalObject(sf::Shape* shape, b2Body * body) : GameObject(shape), body(body)
	{
	}

}