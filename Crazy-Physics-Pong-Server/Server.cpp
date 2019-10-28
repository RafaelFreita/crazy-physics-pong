#include "Server.h"

namespace CPPong {
	Server::Server()
	{
		tcpListener.listen(SERVER_PORT);
		tcpListener.setBlocking(false);
		selector.add(tcpListener);

		listenSocket.setBlocking(false);
		senderSocket.setBlocking(false);
	}

	Server::~Server()
	{
	}

	sf::Socket::Status Server::Bind(unsigned short port)
	{
		return listenSocket.bind(port);
	}

	void Server::CheckNewConnections()
	{
		if (selector.wait(sf::Time(sf::milliseconds(1)))) {
			if (selector.isReady(tcpListener)) {
				sf::TcpSocket* client = new sf::TcpSocket;
				if (tcpListener.accept(*client) == sf::Socket::Status::Done) {
					printf("Adding new client!\n");
					tempTcpSockets.push_back(client);
					selector.add(*client);
				}
				else {
					delete client;
				}
			}
			else {
				std::list<sf::TcpSocket*>::iterator it = tempTcpSockets.begin();
				while (it != tempTcpSockets.end())
				{
					sf::TcpSocket& client = **it;
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive it
						sf::Packet packet;
						if (client.receive(packet) == sf::Socket::Done)
						{
							auto clientIp = client.getRemoteAddress();

							// Client sends the port for UDP connection
							unsigned short clientPort;
							packet >> clientPort;

							printf("Received UDP port from %s:%u!\n", clientIp.toString().c_str(), clientPort);

							ClientAddr* clientAddr = new ClientAddr();
							clientAddr->ip = clientIp;
							clientAddr->port = clientPort;

							sf::UdpSocket* clientSocket = new sf::UdpSocket();
							clientSocket->setBlocking(false);
							clientSocket->bind(sf::Socket::AnyPort);

							clients[clientSocket] = clientAddr;

							// Send back the port for the server's UDP socket
							printf("Sending server UDP port to client at %s:%u!\n", clientIp.toString().c_str(), clientPort);
							packet.clear();
							packet << clientSocket->getLocalPort();
							client.send(packet);

							printf("Removing TCP connection from selector!\n");
							selector.remove(client);
							it = tempTcpSockets.erase(it);

							// Checking if players are already assigned
							if (clientLeft == NULL) { clientLeft = clientAddr; }
							else if (clientRight == NULL) { clientRight = clientAddr; }
						}
					}
					else {
						++it;
					}
				}
			}
		}
	}

	sf::Socket::Status Server::Receive()
	{
		for (auto const& client : clients) {

			sf::IpAddress senderIp;
			unsigned short senderPort;
			size_t receivedSize;

			sf::Socket::Status status = client.first->receive(receivedData, userPacketSize, receivedSize, senderIp, senderPort);

			// HACK: Find a better way to work around empty data
			if (receivedSize != userPacketSize) {
				continue;
			}

			// Check if is a player
			if (clientLeft != NULL) {
				if (clientLeft->ip == senderIp && clientLeft->port == senderPort) {
					std::copy(std::begin(receivedData), std::end(receivedData), std::begin(receivedDataL));
					clientLeftData = reinterpret_cast<GameUserData*>(receivedDataL);
					continue;
				}
			}

			if (clientRight != NULL) {
				if (clientRight->ip == senderIp && clientRight->port == senderPort) {
					std::copy(std::begin(receivedData), std::end(receivedData), std::begin(receivedDataR));
					clientRightData = reinterpret_cast<GameUserData*>(receivedDataR);
					continue;
				}
			}

#pragma region Debugs
#ifdef DEBUG_RECEIVED_PACKET
			printf("[%s:%hu](%zu) - %s\n", senderIp.toString().c_str(), senderPort, receivedSize, receivedData);
#endif
#ifdef DEBUG_INPUT
			printf("ID: %u | UP: %s | DOWN: %s | TYPE: %i\n",
				gameUserData->header.id,
				gameUserData->pressingUp ? "TRUE" : "FALSE",
				gameUserData->pressingDown ? "TRUE" : "FALSE",
				gameUserData->type);
#endif
#ifdef DEBUG_BYTES
			printf("Bytes: ");
			for (char byte : data) {
				printf("%d ", byte);
			}
			printf("\n");
#endif
#pragma endregion

		}

		return sf::Socket::Status::Done;
	}

	void Server::SendState()
	{
		gameState.header.id = gameTick;
		gameTick++;

		char* stateData = reinterpret_cast<char*>(&gameState);
		// For each client socket
		for (auto const& client : clients) {
			if (client.first->send(stateData, packetSize, client.second->ip, client.second->port) != sf::Socket::Status::Done) {
				fprintf(stderr, "ERROR::FAILED TO SEND STATE");
			}
		}
	}

	void Server::Shutdown()
	{
		printf("Shutting down gracefully ...\n");

		// Delete client ports vector
		for (auto const& client : clients) {
			delete client.second;
		}

		listenSocket.unbind();

		printf("Server shut down successfuly!\n");
	}
}