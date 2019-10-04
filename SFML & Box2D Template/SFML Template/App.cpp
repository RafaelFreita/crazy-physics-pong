#include "App.h"

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

		/* --- Box2D Setup --- */
		world = new b2World(gravity);

		/* --- Game Scene Setup */
		// Initialize players
		const static float32 offsetBorder = 64;
		playerL = new Player(world, b2Vec2(offsetBorder, height / 2.f));		// Set left player to center left
		playerR = new Player(world, b2Vec2(static_cast<float32>(width) - offsetBorder, height / 2.f));// Set right player to center right

		// Adding to render list
		renderObjects.push_back(playerL->GetPhysicalObj());
		renderObjects.push_back(playerR->GetPhysicalObj());

		// Adding to physical simulation list
		physicalObjects.push_back(playerL->GetPhysicalObj());
		physicalObjects.push_back(playerR->GetPhysicalObj());

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { playerL->MoveUp();	}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { playerL->MoveDown(); }

		// Player Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) { playerR->MoveUp();	}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { playerR->MoveDown(); }

	}

	void App::Update()
	{
		// Advance world simulation
		world->Step(TIME_STEP, VEL_ITTS, POS_ITTS);

		// Iterate and update objects
		for (PhysicalObject* physicalObject : physicalObjects) {

			// Position
			b2Vec2 pos = physicalObject->body->GetPosition();
			physicalObject->shape->setPosition(pos.x * W2P, pos.y * W2P);

			// Rotation
			float angle = physicalObject->body->GetAngle();
			physicalObject->shape->setRotation(angle * RAD);
		}
	}

	void App::Render()
	{
		// Clear framebuffer
		window->clear(sf::Color::Black);

		// Render objects
		for (GameObject* gameObject : renderObjects) {
			window->draw(*gameObject->shape);
		}

		// Display
		window->display();
	}

	void App::Finish()
	{
		// App
		delete playerL;
		delete playerR;

		// Box2D
		delete world;
		
		// SFML
		delete window;
	}

}