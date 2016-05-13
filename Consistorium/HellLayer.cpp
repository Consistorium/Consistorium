#include "HellLayer.h"

#include "Game/Globals/Constants.h"
#include "Game/Entities/EntityFactory.h"

HellLayer::HellLayer()
{
}

HellLayer::~HellLayer()
{
}

b2Vec2 HellLayer::GetLayerRange()
{
	return Globals::HELL_LAYER_HEIGHT_RANGE;
}

void HellLayer::Generate(Entities::EntityFactory& factory, SpecialPlacesManager& placesManager)
{
	GameEngine::IRenderable *current;

	std::vector<std::vector<Entities::GameEntity*>> cache;
	b2Vec2 currentPosition;
	for (int i = GetLayerRange().x; i >= GetLayerRange().y; i -= Globals::BLOCK_HEIGHT)
	{
		cache.push_back(std::vector<Entities::GameEntity*>());
		for (int j = -Globals::LAYER_WIDTH_IN_BLOCKS / 2; j < Globals::LAYER_WIDTH_IN_BLOCKS / 2; j += Globals::BLOCK_WIDTH)
		{
			std::string current = "Hell";
			currentPosition.x = j;
			currentPosition.y = i;
			Entities::GameEntity* block = factory.createFromName(currentPosition, current);
			cache[cache.size() - 1].emplace_back(block);
		}
	}

	b2Vec2 worldCoords;
	for (int y = 0; y < cache.size(); y++)
	{
		for (int x = cache[y].size() - 1; x >= 0; x--)
		{
			std::shared_ptr<SpecialPlace> place = placesManager.getRandomPlace("Hell");
			if (cache[y][x] == nullptr) { continue; }
			if (rand() % *place->getFrequency() == 1)
			{
				worldCoords = cache[y][x]->getBody()->GetPosition();
				worldCoords.x *= Globals::PIXELS_PER_METER;
				worldCoords.y *= Globals::PIXELS_PER_METER;
				if (cache[y][x] != nullptr) {
					placesManager.spawnPlace(worldCoords, place, factory, cache, b2Vec2(x, y));
				}
			}
		}
	}
}