#include "GameObject.h"

namespace CPPong {

	GameObject::GameObject()
	{
	}


	GameObject::~GameObject()
	{
	}

	GameObject::GameObject(sf::Shape * shape) : shape(shape)
	{
	}

}