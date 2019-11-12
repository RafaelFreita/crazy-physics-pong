#include "Client.h"

namespace CPPong {
	Client::Client()
	{
		if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) {
			fprintf(stderr, "ERROR::FAILED TO BIND PORT\n");
			throw "ERROR::FAILED TO BIND PORT";
		}
		socket.setBlocking(false);

		sf::Packet packet;

		// Send UDP port
		packet << socket.getLocalPort();
		if (tcpSocket.connect(SERVER_IP, SERVER_PORT)) {
			fprintf(stderr, "ERROR::FAILED TO CONNECT TCP\n");
			throw "ERROR::FAILED TO CONNECT TCP";
		}
		tcpSocket.send(packet);

		// Get private UDP port with server
		packet.clear();
		tcpSocket.receive(packet);
		packet >> privateServerPort;
		packet >> clientType;

		printf("%d\n", privateServerPort);
		printf("%i\n", clientType);
	}

	Client::~Client()
	{
	}

	sf::Socket::Status Client::SendPacket()
	{
		return socket.send(reinterpret_cast<char*>(gameUserPacket), userPacketSize, SERVER_IP, privateServerPort);
	}

	sf::Socket::Status Client::Receive()
	{
		sf::IpAddress senderIp;
		unsigned short senderPort;
		size_t receivedSize;

		sf::Socket::Status status = socket.receive(gameStateData, statePacketSize, receivedSize, senderIp, senderPort);

		// HACK: Find a better way to work around empty data
		if (receivedSize == 0) {
			return sf::Socket::Status::Done;
		}

		//printf("[%s:%hu](%zu) - %s\n", senderIp.toString().c_str(), senderPort, receivedSize, receivedData);
		receivedPacketThisFrame = true;

		gameStatePacket = reinterpret_cast<GameStateData*>(gameStateData);

		//printf("ID: %u\n", gameStatePacket->header.id);

		return status;
	}

	void Client::Shutdown()
	{
		printf("Shutting down gracefully ...\n");

		socket.unbind();

		printf("Client connection is over!\n");
	}
}