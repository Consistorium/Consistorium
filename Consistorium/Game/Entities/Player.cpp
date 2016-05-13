#include "Player.h"

#include <iostream>

namespace Entities
{
	Player::Player(b2Body* body, GameEngine::RenderComponent* rc, GameEngine::AnimationComponent* ac)
		: FluentEntity(body, rc, ac),
		actionbarSelected_(0)
	{
		setHealth(100);
	}

	void Player::dig()
	{
	}

	int Player::addToActionbar(Entities::GameEntity* entity)
	{
		for (int i = 0; i < actionbar_.size(); i++)
		{
			auto rc = dynamic_cast<GameEngine::RenderComponent*>(entity->getRenderableComponent());
			if (actionbar_[i]->tryAdd(1, entity, rc))
			{
				return i;
			}
		}

		if (actionbar_.size() >= ACTIONBAR_SIZE)
		{
			return -1;
		}

		auto itemSlot = new Utils::ItemSlot(entity->getSize(), entity->getPosition(), 1);
		itemSlot->getItem().first->setType(entity->getType());
		itemSlot->getItem().second->setTextureName(*entity->getRenderableComponent()->getTextureName());

		actionbar_.push_back(std::shared_ptr<Utils::ItemSlot>(itemSlot));
		return actionbar_.size() - 1;
	}

	int Player::addToInventory(Entities::GameEntity* entity)
	{
		auto itemSlot = new Utils::ItemSlot(entity->getSize(), entity->getPosition());
		for (int i = 0; i < inventory_.size(); i++)
		{
			auto rc = dynamic_cast<GameEngine::RenderComponent*>(entity->getRenderableComponent());
			if (inventory_[i]->tryAdd(1, entity, rc))
			{
				return i;
			}
		}

		if (inventory_.size() >= INVENTORY_SIZE)
		{
			std::cout << "Inventory is full!" << std::endl;
			return -1;
		}

		inventory_.push_back(std::shared_ptr<Utils::ItemSlot>(itemSlot));
		return inventory_.size() - 1;
	}

	std::vector<std::shared_ptr<Utils::ItemSlot>> Player::getInventory()
	{
		return inventory_;
	}

	std::vector<std::shared_ptr<Utils::ItemSlot>> Player::getActionbar()
	{
		return actionbar_;
	}

	Player::~Player()
	{
		actionbar_.clear();
	}
}