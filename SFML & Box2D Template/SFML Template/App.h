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
#include "Ball.h"
#include "Goal.h"
#include "ContactListener.h"

using std::string;

namespace CPPong {

	class App
	{
	private:
		// --- Internal ---

		string title;
		int width;
		int height;

		// --- SFML ---

		sf::RenderWindow* window = NULL;
		sf::Vertex winRectangle[4];
		sf::RenderTexture renderTexture;
		sf::Shader postFxShader;
		sf::RenderStates postFxRenderState;

		// --- Box2D ---

		const b2Vec2 gravity = b2Vec2(0.f, 9.8f);
		b2World* world = NULL;
		b2Body*	 wallBody = NULL;

		// --- Game Variables ---

		Player* playerL = NULL;
		Player* playerR = NULL;
		Ball*	ball = NULL;
		Goal*	goalL = NULL;
		Goal*	goalR = NULL;
		ContactListener contactListenerCallback;

		std::list<GameObject*> renderObjects;
		std::list<PhysicalObject*> dynamicObjects;

		// --- Methods ---

		void HandleInputs();
		void Update();
		void Render();
		void Finish();

		void SetWindowRectangle();

		// Gets the post processings FX shader (motion blur). 
		void GetPostProcessingShader();

		// Sets the contact listener callback.
		void SetContactListeners();

		// Sets wall colliders for borders of the window.
		// Left and right borders are the goals.
		void SetWallCollider();

		void ResetGameState();

	public:
		App(int width, int height, string title);
		~App();

		void Run();
	};

}

#endif