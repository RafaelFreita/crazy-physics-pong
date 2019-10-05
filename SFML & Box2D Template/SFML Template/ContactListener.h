#ifndef H_CL_PLAYER_BALL
#define H_CL_PLAYER_BALL

#include <Box2D/Box2D.h>

namespace CPPong {

	class ContactListener : public b2ContactListener
	{
	public:
		ContactListener();
		~ContactListener();

		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
	};

}

#endif