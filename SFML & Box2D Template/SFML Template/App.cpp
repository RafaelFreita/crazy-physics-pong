#include "App.h"

// Externals
#include <Box2D/Box2D.h>

// STD
#include <iostream>

// Internal
#include "Constants.hpp"

namespace CPPong {

	App::App(int width, int height, string title) : width(width), height(height), title(title)
	{
		/* --- SFML Setup --- */

		// Window settings
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;

		// Initalize window
		window = new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default, settings);
		window->setVerticalSyncEnabled(true);
		window->setFramerateLimit(TARGET_FPS);

		SetWindowRectangle();
		GetPostProcessingShader();

		/* --- Box2D Setup --- */
		world = new b2World(gravity);
		SetContactListeners();

		/* --- Game Scene Setup */
		SetWallCollider();

		// Initialize players
		const static float32 offsetBorder = 64;

		// Set left player to center left
		playerL = new Player(world, b2Vec2(offsetBorder, (float32)height / 2.f));
		// Set right player to center right
		playerR = new Player(world, b2Vec2((float32)width - offsetBorder, (float32)height / 2.f));

		// TODO: Setting player types
		playerL->SetType(PlayerType::Default);
		playerR->SetType(PlayerType::Velcro);

		// Ball
		ball = new Ball(world, b2Vec2((float32)width / 2.f, (float32)height / 2.f));
		//ball->ApplyLinearImpulseToCenter(b2Vec2(5.0f, .0f), false);

		// Adding to render list
		renderObjects.push_back(playerL);
		renderObjects.push_back(playerR);
		renderObjects.push_back(ball);

		// Adding to physical simulation list
		dynamicObjects.push_back(playerL);
		dynamicObjects.push_back(playerR);
		dynamicObjects.push_back(ball);
	}

	App::~App()
	{
		Finish();
	}

	void App::Run()
	{
		while (window->isOpen())
		{
			HandleInputs();
			Update();
			Render();
		}
	}

	void App::HandleInputs()
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();
			else if (event.key.code == sf::Keyboard::Escape) window->close();
		}

		// --- Players Inputs ---
		// Player Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { playerL->MoveUp(); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { playerL->MoveDown(); }

		// Player Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { playerR->MoveUp(); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { playerR->MoveDown(); }

	}

	void App::Update()
	{
		// Advance world simulation
		world->Step(TIME_STEP, VEL_ITTS, POS_ITTS);

		// Iterate and update objects
		for (PhysicalObject* physicalObject : dynamicObjects) {

			// Manual check
			physicalObject->CheckPhysics();

			// Set Position
			b2Vec2 pos = physicalObject->GetPos();
			physicalObject->SetPos(pos.x * W2P, pos.y * W2P);

			// Set Rotation
			physicalObject->SetRot(physicalObject->GetAngle() * RAD);
		}

		// Check if goals were scored
		if (goalL->GetWasScored()) {
			goalL->ClearScore();
			ResetGameState();
		}
		else if (goalR->GetWasScored()){
			goalR->ClearScore();
			ResetGameState();
		}
	}

	void App::Render()
	{
		// Clear framebuffer
		renderTexture.clear(sf::Color::Black);

		// Render objects
		for (GameObject* gameObject : renderObjects) {
			renderTexture.draw(*gameObject->GetShape());
		}

		// Display texture
		renderTexture.display();

		// Clear window
		window->clear(sf::Color::Black);

		// Blit texture to window
		window->draw(winRectangle, 4, sf::TrianglesStrip, postFxRenderState);

		// Display
		window->display();
	}

	void App::Finish()
	{
		// App
		delete ball;
		delete playerL;
		delete playerR;
		delete goalL;
		delete goalR;

		// Box2D
		delete world;

		// SFML
		delete window;
	}

	void App::SetWindowRectangle()
	{
		winRectangle[0] = sf::Vector2f(0, 0);
		winRectangle[1] = sf::Vector2f(width, 0);
		winRectangle[2] = sf::Vector2f(0, height);
		winRectangle[3] = sf::Vector2f(width, height);
	}

	void App::GetPostProcessingShader()
	{
		if (!renderTexture.create(width, height, true)) {
			printf("ERROR::Couldn't create render texture");
			return;
		}

		if (!sf::Shader::isAvailable()) {
			printf("ERROR::Shaders not available!");
			return;
		}

		if (!postFxShader.loadFromFile("shaders/postfx.glsl", sf::Shader::Fragment)) {
			printf("ERROR::Couldn't load Post FX shader!");
			return;
		}

		postFxShader.setUniform("texture", renderTexture.getTexture());

		postFxRenderState = sf::RenderStates(&postFxShader);

	}

	void App::SetWallCollider()
	{
		b2BodyDef wallBodyDef;
		wallBodyDef.type = b2_staticBody;
		wallBodyDef.position.Set(0, 0); // (0, 0) is Top Left corner

		wallBody = world->CreateBody(&wallBodyDef);

		b2EdgeShape wallEdge;
		b2FixtureDef boxShapeDef;
		boxShapeDef.filter.categoryBits = ET_Wall;
		boxShapeDef.filter.categoryBits = ET_Any;

		boxShapeDef.shape = &wallEdge;

		// Define wall edges
		static const b2Vec2 topLeft = b2Vec2(0.f / W2P, 0.f / W2P);
		static const b2Vec2 topRight = b2Vec2(width / W2P, 0.f / W2P);
		static const b2Vec2 bottomLeft = b2Vec2(0.f / W2P, height / W2P);
		static const b2Vec2 bottomRight = b2Vec2(width / W2P, height / W2P);

		// Top Wall
		wallEdge.Set(topLeft, topRight);
		wallBody->CreateFixture(&boxShapeDef);

		// Bottom Wall
		wallEdge.Set(bottomLeft, bottomRight);
		wallBody->CreateFixture(&boxShapeDef);

		// Left Wall/Goal
		goalL = new Goal(world, topLeft, bottomLeft, true);

		// Right Wall/Goal
		goalR = new Goal(world, topRight, bottomRight, false);
	}

	void App::ResetGameState()
	{
		playerL->Reset();
		playerR->Reset();
		ball->Reset();
	}

	void App::SetContactListeners()
	{
		world->SetContactListener(&contactListenerCallback);
	}

}