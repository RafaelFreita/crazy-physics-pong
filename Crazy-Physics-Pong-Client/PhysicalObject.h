#ifndef H_PHYSICSOBJECT
#define H_PHYSICSOBJECT

#include <Box2D/Dynamics/b2Body.h>

#include "GameObject.h"
#include "EntityTypes.hpp"

namespace CPPong {

	class PhysicalObject : public GameObject
	{
	protected:
		PhysicalObject(EntityType type = ET_Any);

		EntityType type = ET_Any;
		b2Body* body = NULL;
	public:
		PhysicalObject(sf::Shape* shape, b2Body* body, EntityType type = ET_Any);
		~PhysicalObject();

		inline void ApplyLinearImpulseToCenter(const b2Vec2& impulse, bool wake) { body->ApplyLinearImpulseToCenter(impulse, wake); }

		inline const EntityType GetType() { return type; }
		inline const b2Vec2& GetPos() { return body->GetPosition(); }
		inline const float& GetAngle() { return body->GetAngle(); }
		inline const b2Vec2& GetVel() { return body->GetLinearVelocity(); }

		inline void SetPhysicalPos(b2Vec2 pos) { body->SetTransform(pos, body->GetAngle()); }

		virtual void CheckPhysics() = 0;
	};

}

#endif