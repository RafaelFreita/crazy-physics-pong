#include "GameObject.h"

namespace CPPong {

	GameObject::GameObject(sf::Shape * shape) : shape(shape)
	{
	}

	GameObject::~GameObject()
	{
		delete shape;
	}

}