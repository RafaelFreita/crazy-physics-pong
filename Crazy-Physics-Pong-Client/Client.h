#ifndef H_CLIENT
#define H_CLIENT

// Externals
#include <SFML/Network.hpp>

// STD
#include <stdio.h>
#include <iostream>
#include <string>

// Internal
#include "NetworkTypes.hpp"

// Using
using std::string;

// Consts
const sf::IpAddress SERVER_IP = "127.0.0.1";
const UINT8 SERVER_PORT = 31415;

namespace CPPong {

	class Client
	{
	private:
		sf::TcpSocket tcpSocket;
		sf::UdpSocket socket;

		static const size_t statePacketSize = sizeof(GameStateData);
		static const size_t userPacketSize = sizeof(GameUserData);

		char gameStateData[statePacketSize] = {};

		GameStateData* gameStatePacket = new GameStateData();
		GameUserData* gameUserPacket = new GameUserData();

		unsigned short privateServerPort = 0;

	public:
		Client();
		~Client();
		
		sf::Socket::Status SendPacket();

		sf::Socket::Status Receive();

		void Shutdown();

		inline GameStateData* GetLatestState() { return gameStatePacket; }

		inline void SetUserPressingUp(bool value) { gameUserPacket->pressingUp = value; }
		inline void SetUserPressingDown(bool value) { gameUserPacket->pressingDown = value; }
		inline void SetUserType(char value) { gameUserPacket->type = value; }
		inline void SetUserPacketID(unsigned int value) { gameUserPacket->header.id = value; }

	};

}

#endif