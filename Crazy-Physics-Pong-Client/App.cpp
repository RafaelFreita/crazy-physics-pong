#include "App.h"

// Externals
#include <Box2D/Box2D.h>
#include <SFML/System.hpp>

// STD
#include <Windows.h>
#include <iostream>
#include <random>

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

		// Init Font
		if (!mainFont.loadFromFile("fonts/arcade.ttf")) {
			printf("ERROR::Failed to load font!");
		}

		// Init window vertices (for framebuffer)
		SetWindowRectangle();

		// Set shader
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
		playerR->SetType(PlayerType::Default);

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

		// Random seed
		srand(time(0));

		// Reseting state to be sure everything is right
		Reset();

		lastTickClock = Clock::now();
	}

	App::~App()
	{
		Finish();
	}

	void App::Run()
	{
		window->setActive(false);

		sf::Thread renderThread(&App::Render, this);
		renderThread.launch();

		// Set as focused window
		window->requestFocus();

		lastFrameClock = Clock::now();

		while (window->isOpen())
		{
			newFrameClock = Clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(newFrameClock - lastTickClock).count() / 1000.0f;
			lastFrameClock = newFrameClock;

			auto timeSinceLastTick = std::chrono::duration_cast<std::chrono::milliseconds>(newFrameClock - lastTickClock).count();

			client.SetReceivedPacket(false);
			Update();

			if (timeSinceLastTick > MILLI_PER_TICK) {
				lastTickClock = newFrameClock;

				HandleInputs();
				SendInputs();

				if (!client.GetReceivePacket()) {
					Predict();
				}
			}

		}
	}

	void App::HandleInputs()
	{
		sf::Event event;

		while (window->pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;

			case sf::Event::GainedFocus:
				windowFocused = true;
				break;

			case sf::Event::LostFocus:
				windowFocused = false;
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) window->close();
				break;

			default:
				break;
			}
		}

		// --- Players Inputs ---
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && windowFocused) { client.SetUserPressingUp(true); }
		else { client.SetUserPressingUp(false); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && windowFocused) { client.SetUserPressingDown(true); }
		else { client.SetUserPressingDown(false); }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && windowFocused) { client.SetUserType(PlayerType::Default); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && windowFocused) { client.SetUserType(PlayerType::Wood); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && windowFocused) { client.SetUserType(PlayerType::Rubber); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && windowFocused) { client.SetUserType(PlayerType::Velcro); }

		// Rotators
		/*
		static bool isPressingRotators;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isPressingRotators) {
			isPressingRotators = true;
			rotatorsOn = !rotatorsOn;

			if (rotatorsOn) {
				rotatorTop = new Rotator(world, b2Vec2((float32)width / 2.f, (float32)(height / 2.f) - (height / 4)), 5.f);
				rotatorBottom = new Rotator(world, b2Vec2((float32)width / 2.f, (float32)(height / 2.f) + (height / 4)), -5.f);

				renderObjects.push_back(rotatorTop);
				renderObjects.push_back(rotatorBottom);

				dynamicObjects.push_back(rotatorTop);
				dynamicObjects.push_back(rotatorBottom);
			}
			else {
				rotatorTop->Destroy(world);
				rotatorBottom->Destroy(world);

				// Popping for both rotators
				renderObjects.pop_back();
				renderObjects.pop_back();

				dynamicObjects.pop_back();
				dynamicObjects.pop_back();

				delete rotatorTop;
				delete rotatorBottom;
			}
		}
		else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { isPressingRotators = false; }
		*/

		// Return to reset
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) { Reset(); }

	}

	void App::Update()
	{
		// Receive new game state
		if (client.Receive() != sf::Socket::Status::Done) {
			throw new std::exception("ERROR::FAILED TO RECEIVE DATA");
		}

		if (client.GetReceivePacket()) {
			UpdateState();
			Conciliate();
		}

	}

	void App::UpdateState()
	{
		GameStateData* newState = client.GetLatestState();

		// Player L
		playerL->SetPhysicalPos(b2Vec2(newState->playerLX, newState->playerLY));
		playerL->SetPos(newState->playerLX * W2P, newState->playerLY * W2P);
		playerL->SetVel(newState->playerLVelX, newState->playerLVelY);
		if (playerL->GetPlayerType() != newState->playerLType) {
			playerL->SetType((PlayerType)newState->playerLType);
		}

		// Player R
		playerR->SetPhysicalPos(b2Vec2(newState->playerRX, newState->playerRY));
		playerR->SetPos(newState->playerRX * W2P, newState->playerRY * W2P);
		playerR->SetVel(newState->playerRVelX, newState->playerRVelY);
		if (playerR->GetPlayerType() != newState->playerRType) {
			playerR->SetType((PlayerType)newState->playerRType);
		}

		// Ball
		ball->SetPhysicalPos(b2Vec2(newState->ballX, newState->ballY));
		ball->SetPos(newState->ballX * W2P, newState->ballY * W2P);
		ball->SetDirection(newState->ballDirX, newState->ballDirY);
		ball->SetAcceleration(newState->ballAccX, newState->ballAccY);

		// Goals
		playerLPoints = newState->playerLGoals;
		playerRPoints = newState->playerRGoals;
	}

	void App::Predict()
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
	}

	void App::PredictInput(GameUserData gameUserData)
	{
		// 0 is left, 1 is right
		// Left
		if (client.GetClientType() == 0) {
			if (gameUserData.pressingUp) { playerL->MoveUp(); }
			if (gameUserData.pressingDown) { playerL->MoveDown(); }
		}
		// Right
		else if (client.GetClientType() == 1) {
			if (gameUserData.pressingUp) { playerR->MoveUp(); }
			if (gameUserData.pressingDown) { playerR->MoveDown(); }
		}

		Predict();
	}

	void App::Conciliate()
	{
		GameStateData* newState = client.GetLatestState();

		// 0 is left, 1 is right
		// If input id is the same or less then the last one server recognized
		// Remove it

		auto it = userInputsBuffer.begin();
		bool itRemoved = false;

		while (it != userInputsBuffer.end()) {

			itRemoved = false;

			// Left
			if (client.GetClientType() == 0) {
				if (it->header.id <= newState->lastIdL) {
					it = userInputsBuffer.erase(it);
					itRemoved = true;
				}
			}
			// Right
			else if (client.GetClientType() == 1) {
				if (it->header.id <= newState->lastIdR) {
					it = userInputsBuffer.erase(it);
					itRemoved = true;
				}
			}

			// Increment
			if (!itRemoved) ++it;
		}

		// Predict all inputs that still exist
		it = userInputsBuffer.begin();
		while (it != userInputsBuffer.end()) {
			PredictInput(*it);

			++it;
		}
	}

	void App::Render()
	{
		while (window->isOpen())
		{
			// Clear framebuffer
			renderTexture.clear(sf::Color::Black);

			// Render objects
			for (GameObject* gameObject : renderObjects) {
				renderTexture.draw(*gameObject->GetShape());
			}

			// Render UI
			RenderUI();

			// Display texture
			renderTexture.display();

			// Clear window
			window->clear(sf::Color::Black);

			// Blit texture to window
			window->draw(winRectangle, 4, sf::TrianglesStrip, postFxRenderState);

			// Display
			window->display();
		}
	}

	void App::Finish()
	{
		client.Shutdown();

		// App
		delete rotatorBottom;
		delete rotatorTop;
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

	void App::SendInputs()
	{
		// Send data
		static unsigned int packetId = 0;
		client.SetUserPacketID(packetId);
		++packetId;

		userInputsBuffer.push_back(client.GetUserPacket());

		if (client.SendPacket() != sf::Socket::Status::Done) {
			fprintf(stderr, "ERROR::FAILED TO SEND PACKET");
		}
	}

	void App::Reset()
	{
		// Randomizing start
		ResetGameState((rand() % 2) == 1);
		playerLPoints = 0;
		playerRPoints = 0;
	}

	void App::RenderUI()
	{
		static char pointsString[2] = { '9','9' };

		sf::Text pointsText(sf::String(pointsString), mainFont, 64U);
		pointsText.setFillColor(sf::Color::White);
		pointsText.setOrigin(16.f, 0.f); // Why these numbers?? I don't know, but they work!

		const static int horizontalPadding = 64;
		// Left
		snprintf(pointsString, 10, "%i", playerLPoints);
		pointsText.setString(sf::String(pointsString));
		pointsText.setPosition(width / 2 - horizontalPadding, 0);
		renderTexture.draw(pointsText);

		// Right
		snprintf(pointsString, 10, "%i", playerRPoints);
		pointsText.setString(sf::String(pointsString));
		pointsText.setPosition(width / 2 + horizontalPadding, 0);
		renderTexture.draw(pointsText);
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
		if (!renderTexture.create(width, height, sf::ContextSettings(0U, 0U, 4U))) {
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

	void App::ResetGameState(bool leftScored)
	{
		playerL->Reset();
		playerR->Reset();
		goalL->ClearScore();
		goalR->ClearScore();

		ball->ResetToSide(!leftScored);
	}

	void App::SetContactListeners()
	{
		world->SetContactListener(&contactListenerCallback);
	}

}