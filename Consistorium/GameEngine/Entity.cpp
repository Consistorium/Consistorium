#include "Entity.h"

namespace Entities
{
	b2Vec2 Entity::getPosition() { return position_; }
	
	Entity* Entity::setPosition(b2Vec2 value) 
	{
		position_ = value;
		return this;
	};
}