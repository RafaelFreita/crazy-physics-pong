#include "App.h"

int main(int argc, char* argv[])
{

	std::string ip = (argc == 2) ? argv[1] : "127.0.0.1";

	CPPong::App app(1280, 720, "Crazy Physics Pong", sf::IpAddress(ip));

	app.Run();

	return EXIT_SUCCESS;
}
