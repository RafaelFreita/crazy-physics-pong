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
#include "Rotator.h"
#include "ContactListener.h"
#include "Client.h"
#include <chrono>

using std::string;

typedef std::chrono::high_resolution_clock Clock;

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
		sf::Font mainFont;
		bool windowFocused = true;

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
		Rotator* rotatorTop = NULL;
		Rotator* rotatorBottom = NULL;
		bool rotatorsOn = false;
		ContactListener contactListenerCallback;
		int playerLPoints = 0;
		int playerRPoints = 0;

		std::list<GameObject*> renderObjects;
		std::list<PhysicalObject*> dynamicObjects;

		// --- NETWORK ---
		unsigned int gameTick = 0;
		Client client;
		const short int TPS = 60;
		const double MILLI_PER_TICK = 1000.0 / TPS;
		std::chrono::time_point<std::chrono::steady_clock> lastTickClock;
		std::chrono::time_point<std::chrono::steady_clock> newFrameClock;
		std::chrono::time_point<std::chrono::steady_clock> lastFrameClock;
		float deltaTime;

		std::list<GameUserData> userInputsBuffer;

		// --- Methods ---

		void HandleInputs();
		void Update();
		void Render();
		void RenderUI();
		void Finish();
		void SendInputs();

		void UpdateState();
		void Predict();
		void PredictInput(GameUserData gameUserData);
		// Predict all buffered inputs
		void Conciliate();

		// Reset game state to start
		void Reset();
		// Soft reset after goal
		void ResetGameState(bool leftScored);

		// Init window vertices for framebuffer
		void SetWindowRectangle();

		// Gets the post processings FX shader (motion blur). 
		void GetPostProcessingShader();

		// Sets the contact listener callback.
		void SetContactListeners();

		// Sets wall colliders for borders of the window.
		// Left and right borders are the goals.
		void SetWallCollider();


	public:
		App(int width, int height, string title, sf::IpAddress ip);
		~App();

		void Run();
	};

}

#endif