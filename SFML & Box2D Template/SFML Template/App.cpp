#include "App.h"

#include <iostream>

#include <Box2D/Box2D.h>

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


		/* --- TESTING --- */
		sf::RectangleShape* rectangle = new sf::RectangleShape(sf::Vector2f(32.f, 32.f));
		rectangle->setOrigin(16.f, 16.f);
		rectangle->setFillColor(sf::Color::Green);
		rectangle->setPosition(width / 2.0f, height / 2.0f);

		/*GameObject* testGO = new GameObject(rectangle);
		gameObjects.push_back(testGO);*/

		b2BodyDef bDef;
		bDef.position = b2Vec2(width / 2 / W2P, height / 2 / W2P);
		bDef.type = b2_dynamicBody;
		b2Body* tBody = world->CreateBody(&bDef);

		b2PolygonShape tShape;
		tShape.SetAsBox((32.f / 2) / W2P, (32.f / 2) / W2P);
		b2FixtureDef tFixtureDef;
		tFixtureDef.density = 1.f;
		tFixtureDef.friction = 0.7f;
		tFixtureDef.shape = &tShape;
		tBody->CreateFixture(&tFixtureDef);

		PhysicalObject* pObject = new PhysicalObject(rectangle, tBody);
		gameObjects.push_back(pObject);
		physicalObjects.push_back(pObject);
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
			if (event.type == sf::Event::Closed)			 window->close();
			else if (event.key.code == sf::Keyboard::Escape) window->close();
		}
	}

	void App::Update()
	{
		static sf::Clock clock;
		std::cout << clock.getElapsedTime().asSeconds() << "\n";

		world->Step(TIME_STEP, VEL_ITTS, POS_ITTS);

		for (PhysicalObject* physicalObject : physicalObjects) {
			b2Vec2 pos = physicalObject->body->GetPosition();
			physicalObject->shape->setPosition(pos.x * W2P, pos.y * W2P);

			float angle = physicalObject->body->GetAngle();
			physicalObject->shape->setRotation(180.0f * angle / PI);
		}
	}

	void App::Render()
	{
		// Clear framebuffer
		window->clear(sf::Color::Black);

		// Render objects
		for (GameObject* gameObject : gameObjects) {
			window->draw(*gameObject->shape);
		}

		// Display
		window->display();
	}

	void App::Finish()
	{
		delete window;
		delete world;
	}

}