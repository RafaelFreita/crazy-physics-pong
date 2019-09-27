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
#include "GameObject.h"
#include "PhysicalObject.h"

using std::string;

namespace CPPong {

	class App
	{
	private:
		// Constants
		/* World unit to Pixel unit. */
		const float W2P = 16.f;

		/* Physics time step. */
		const float TIME_STEP = 1.f / 120.f;

		/* Physics velocity iterations. */
		const int VEL_ITTS = 8;

		/* Physics position iterations. */
		const int POS_ITTS = 3;

		/* PI constant. */
		const int PI = 3.1415f;

		// Internal
		string title;
		int width;
		int height;

		// SFML
		sf::RenderWindow* window;

		// Box2D
		const b2Vec2 gravity = b2Vec2(0.f, 9.8f);
		b2World* world;

		// Methods
		void HandleInputs();
		void Update();
		void Render();
		void Finish();
	public:
		App(int width, int height, string title);
		~App();

		void Run();

		std::list<GameObject*> gameObjects;
		std::list<PhysicalObject*> physicalObjects;
	};

}

#endif