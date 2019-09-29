#include "PhysicalObject.h"

namespace CPPong {

	PhysicalObject::PhysicalObject(sf::Shape* shape, b2Body* body) : GameObject(shape), body(body)
	{
	}

	PhysicalObject::~PhysicalObject()
	{
		// TODO: Clear pointer?
		// delete body;
	}

}