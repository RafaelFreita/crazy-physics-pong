#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT

#include <SFML/Graphics/Shape.hpp>

namespace CPPong {

	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		GameObject(sf::Shape* shape);

		sf::Shape* shape;
	};

}

#endif