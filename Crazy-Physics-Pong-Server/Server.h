#ifndef H_SERVER
#define H_SERVER

// External
#include <SFML/network.hpp>

// STD
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

// Internal
#include "NetworkTypes.hpp"

// Usings
using std::string;

// Consts
const UINT8 SERVER_PORT = 31415;

//#define DEBUG_RECEIVED_PACKET
//#define DEBUG_INPUT
//#define DEBUG_BYTES

namespace CPPong {

	struct ClientAddr {
		sf::IpAddress ip;
		unsigned short port;
	};

	class Server
	{
	private:
		sf::TcpListener tcpListener;
		sf::SocketSelector selector;

		sf::UdpSocket listenSocket;
		sf::UdpSocket senderSocket;

		// Mapping IP address to Port
		std::map<sf::UdpSocket*, ClientAddr*> clients;
		//std::map<sf::IpAddress, std::map<unsigned short, sf::UdpSocket*>*> clients;
		std::list<sf::TcpSocket*> tempTcpSockets;

		ClientAddr* clientLeft = NULL;
		ClientAddr* clientRight = NULL;

		GameUserData* clientLeftData = NULL;
		GameUserData* clientRightData = NULL;

		GameStateData gameState = {};

		static const size_t packetSize = sizeof(GameStateData);
		static const size_t userPacketSize = sizeof(GameUserData);
		char receivedData[userPacketSize] = {};

		char receivedDataL[userPacketSize] = {};
		char receivedDataR[userPacketSize] = {};

		unsigned int gameTick = 0;

		bool running = true;

	public:
		Server();
		~Server();

		sf::Socket::Status Bind(unsigned short port);

		void CheckNewConnections();
		sf::Socket::Status Receive();

		void SendState();

		void Shutdown();

		inline GameUserData* GetPlayerLeft() { return clientLeftData; }
		inline GameUserData* GetPlayerRight() { return clientRightData; }

		inline void SetPlayerLeftPos(float x, float y) { gameState.playerLX = x; gameState.playerLY = y; }
		inline void SetPlayerRightPos(float x, float y) { gameState.playerRX = x; gameState.playerRY = y; }
		inline void SetBallPos(float x, float y) { gameState.ballX = x; gameState.ballY = y; }

		inline void SetPlayerLType(char type) { gameState.playerLType= type; }
		inline void SetPlayerRType(char type) { gameState.playerRType = type; }

		inline void SetPlayerLGoals(unsigned int goals) { gameState.playerLGoals = goals; }
		inline void SetPlayerRGoals(unsigned int goals) { gameState.playerRGoals = goals; }
	};

}

#endif