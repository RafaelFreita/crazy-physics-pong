struct PacketHeader {
	unsigned int id = 0;
};

struct GameStateData {
	PacketHeader header;

	float ballX;
	float ballY;

	float playerLX;
	float playerLY;

	float playerRX;
	float playerRY;

	char playerLType;
	char playerRType;

	unsigned int playerLGoals;
	unsigned int playerRGoals;
};

// GAME USER DATA BYTE
// 7 6 5 4 3 2 1
// X X X | | | |
//       | | | > pressing up
//       | | --> pressing down 
//       | ----> typeMask bit 1
//       ------> typeMask bit 2

struct GameUserData {
	PacketHeader header;
	bool pressingUp = false;
	bool pressingDown = false;
	char type = 0;
};