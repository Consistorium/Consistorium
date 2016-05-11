#pragma once

#include "IWorldLayer.h"

class UndergroundLayer : public IWorldLayer
{
public:
	UndergroundLayer();

	~UndergroundLayer();

	b2Vec2 GetLayerRange() override;

	void Generate(EntityManager& entityManager, SpecialPlacesManager& placesManager) override;
};

