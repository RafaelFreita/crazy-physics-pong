#ifndef H_PLAYER
#define H_PLAYER

#include "PhysicalObject.h"

#include <map>

namespace CPPong {

	enum PlayerType {
		Default,
		Wood,
		Rubber,
		Velcro
	};

	struct PlayerSpecs {
		float ballRicochetFactor;
		float ballEffectFactor;
	};

	class Player : public PhysicalObject
	{
	private:
		const b2Vec2	size = { 16.f, 64.f };
		const float32	movementForce = 2.f;
		b2Vec2			initPos;

		float ballRicochetFactor = 0.5;
		float ballEffectFactor = 0.5f;

		PlayerType playerType;

	public:
		static std::map<PlayerType, PlayerSpecs> const playerTypeMap;

		Player(b2World* world, const b2Vec2& initPos);
		~Player();

		inline void MoveUp() { body->ApplyLinearImpulseToCenter(b2Vec2(0.f, -movementForce), false); }
		inline void MoveDown() { body->ApplyLinearImpulseToCenter(b2Vec2(0.f, +movementForce), false); }

		// Locking player to Y axis.
		void CheckPhysics();

		void SetType(PlayerType playerType);
		inline PlayerType GetPlayerType() { return playerType; }

		inline float GetBallEffectFactor() { return ballEffectFactor; }
		inline float GetBallRicochetFactor() { return ballRicochetFactor; }

		inline void SetVel(float x, float y) { body->SetLinearVelocity(b2Vec2(x, y)); }

		void Reset();
	};

}

#endif