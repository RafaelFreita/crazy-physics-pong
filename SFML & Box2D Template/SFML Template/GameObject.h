#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT

#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

namespace CPPong {

	class GameObject
	{
	protected:
		GameObject();

		sf::Shape* shape = NULL;
	public:
		GameObject(sf::Shape* shape);
		~GameObject();

		inline sf::Shape* GetShape() { return shape; }

		inline void SetPos(const float& x, const float& y)	{ shape->setPosition(sf::Vector2f(x, y)); }
		inline void SetPos(const sf::Vector2f& position)	{ shape->setPosition(position); }
		inline void SetRot(const float& rotation)			{ shape->setRotation(rotation); }
	};

}

#endif