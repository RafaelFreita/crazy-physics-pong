#ifndef APP_H
#define APP_H

// Libraries
#include <SFML/Graphics.hpp>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2World.h>

// STD
#include <string>
#include <list>

// Internal
#include "Player.h"

using std::string;

namespace CPPong {

	class App
	{
	private:
		// Internal
		string title;
		int width;
		int height;

		// SFML
		sf::RenderWindow* window = NULL;

		// Box2D
		const b2Vec2 gravity = b2Vec2(0.f, 9.8f);
		b2World* world = NULL;

		// Game Variables
		Player* playerL = NULL;
		Player* playerR = NULL;

		// Methods
		void HandleInputs();
		void Update();
		void Render();
		void Finish();
	public:
		App(int width, int height, string title);
		~App();

		void Run();

		std::list<GameObject*> renderObjects;
		std::list<PhysicalObject*> physicalObjects;
	};

}

#endif