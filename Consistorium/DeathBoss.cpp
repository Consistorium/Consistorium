#include "DeathBoss.h"

#include <Game\Globals\Constants.h>
#include <Game\Entities\EntityTypes.h>
#include <Game\Utils\Directions.h>

namespace Entities
{
	DeathBoss::DeathBoss(
		b2Body* body,
		GameEngine::RenderComponent* rc,
		GameEngine::AnimationComponent* ac)
		: FluentEntity(body, rc, ac)
	{
		attackTimer_.Reset();
	}

	void DeathBoss::iterateAI(FluentEntity& player, float dt)
	{
		setXDirection(scan(player));
		if (getXDirection() != 0)
		{
			animationManager_.setAnimation("Move", true);
			move();
		}
		else
		{
			animationManager_.revertAnimation();
		}

		tryAttack(player);
	}

	bool DeathBoss::locatedEnemy(b2Vec2 position, b2Vec2 enemyPosition, float range)
	{
		return abs(position.x - enemyPosition.x) <= range;
	}

	int DeathBoss::scan(FluentEntity& player)
	{
		if (locatedEnemy(getPosition(), player.getPosition(), scanRange_) &&
			(int)player.getUserData() == (int)EntityTypes::Player)
		{
			float delta = ceil((getSize().x / 2 + player.getSize().x / 2) / Globals::PIXELS_PER_METER);
			if (getPosition().x - player.getPosition().x > delta)
			{
				return LEFT;
			}
			else if (player.getPosition().x - getPosition().x > delta)
			{
				return RIGHT;
			}
		}

		return 0;
	}

	void DeathBoss::tryAttack(FluentEntity& player)
	{
		float deltaX = ceil(getPosition().x - player.getPosition().x),
			deltaY = ceil(getPosition().y - player.getPosition().y);
		if (deltaX < range_ && deltaY < range_)
		{
			if (attackTimer_.GetMilliseconds()  > 1000 / haste_)
			{
				animationManager_.setAnimation("Attack", false);
				attackTimer_.Reset();
				player.setHealth(player.getHealth() - damage_);
			}
		}
	}
}