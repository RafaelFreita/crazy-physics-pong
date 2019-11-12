struct PacketHeader {
	unsigned int id = 0;
};

struct GameStateData {
	PacketHeader header;

	unsigned int lastIdL = 0;
	unsigned int lastIdR = 0;

	float ballX;
	float ballY;
	float ballDirX;
	float ballDirY;
	float ballAccX;
	float ballAccY;

	float playerLX;
	float playerLY;
	float playerLVelX;
	float playerLVelY;

	float playerRX;
	float playerRY;
	float playerRVelX;
	float playerRVelY;

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