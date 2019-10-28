#ifndef H_GOAL
#define H_GOAl

#include "PhysicalObject.h"

namespace CPPong {

	class Goal : public PhysicalObject
	{
	private:
		bool isLeft;
		bool wasScored = false;

	public:
		Goal(b2World* world, const b2Vec2& p1, const b2Vec2& p2, bool isLeft);
		~Goal();

		void CheckPhysics();

		inline bool GetIsLeft() { return isLeft; }
		inline bool GetWasScored() { return wasScored; }

		inline void Score() { wasScored = true; }
		inline void ClearScore() { wasScored = false; }
	};

}

#endif