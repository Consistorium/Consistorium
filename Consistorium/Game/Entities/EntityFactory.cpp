#include <vector>
#include <Game\Globals\Constants.h>

#include "Capsule.h"
#include "EntityIndexesEnum.h"

#include "EntityFactory.h"

namespace Entities
{
	const float PLAYER_JUMP_POWER = 5.0;

	EntityFactory::EntityFactory(EntityManager& entityManager)
		: entityManager_(entityManager)
	{
	}

	b2Body* EntityFactory::createDynamicEntityBody(b2Vec2 position, float width, float height, float friction) {
		b2Body* body = Capsule::create(entityManager_.getWorld(), position, width, height);
		b2Fixture* fixture = body->GetFixtureList();
		while (fixture != nullptr) {
			fixture->SetFriction(friction);
			fixture->SetDensity(1.0);
			fixture = fixture->GetNext();
		}

		//add foot sensor fixture
		float sensorDim = 0.1;
		b2Vec2 sensorCentre(0, -height / 2 - sensorDim / 2);
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(sensorDim, sensorDim, sensorCentre, 0);

		b2FixtureDef SensorFixtureDef;
		SensorFixtureDef.shape = &polygonShape;
		SensorFixtureDef.isSensor = true;
		b2Fixture* footSensorFixture = body->CreateFixture(&SensorFixtureDef);
		footSensorFixture->SetUserData((void*)EntityIndexes::FootSensor);

		return body;
	}

	b2Body* EntityFactory::createStaticEntityBody(b2Vec2 position, float width, float height)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(position.x / Globals::PIXELS_PER_METER, position.y / Globals::PIXELS_PER_METER);
		b2Body* body = entityManager_.getWorld()->CreateBody(&bodyDef);

		b2ChainShape chainShape;

		b2Vec2 vertices[8];
		vertices[7] = b2Vec2(-width / 2, -height / 2 + 0.01); // middle bottom left
		vertices[6] = b2Vec2(-width / 2, +height / 2 - 0.01); // middle top left
		vertices[5] = b2Vec2((-width / 2) + 0.03, +height / 2); // top left
		vertices[4] = b2Vec2(+width / 2 - 0.03, +height / 2); // top right
		vertices[3] = b2Vec2(+width / 2, +height / 2 - 0.01); // middle top right
		vertices[2] = b2Vec2(+width / 2, -height / 2 + 0.01); // middle bottom right
		vertices[1] = b2Vec2(+width / 2 - 0.03, -height / 2); // bottom right
		vertices[0] = b2Vec2((-width / 2) + 0.03, -height / 2); // bottom left
		chainShape.CreateLoop(vertices, 8);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &chainShape;

		body->CreateFixture(&fixtureDef);

		return body;
	}

	Player* EntityFactory::createPlayer(b2Vec2 position, std::string modelName, float health)
	{
		float playerWidth = Globals::DEFAULT_ENTITY_WIDTH / Globals::PIXELS_PER_METER,
			playerHeight = Globals::DEFAULT_ENTITY_HEIGHT / Globals::PIXELS_PER_METER;

		b2Body* body = Capsule::create(entityManager_.getWorld(), position, playerWidth, playerHeight);
		body->SetUserData((void*)EntityIndexes::Player);

		b2Fixture* fixture = body->GetFixtureList();

		GameEngine::RenderComponent rc(Globals::MODELS_LOCATION + "Player/Idle__001.png", b2Vec2(Globals::DEFAULT_ENTITY_WIDTH, Globals::DEFAULT_ENTITY_HEIGHT), body);
		GameEngine::AnimationComponent ac(Globals::MODELS_LOCATION + "Player", "Idle", 40, rc.getTextureName());
		Player* player = new Player(rc, ac, PLAYER_JUMP_POWER);
		player->setHealth(health);
		player->setMaxHealth(health);

		entityManager_.addToWorld(player);
		
		return player;
	}

	Block* EntityFactory::createBlock(b2Vec2 position, std::string modelName)
	{
		float blockHeight = Globals::BLOCK_HEIGHT / Globals::PIXELS_PER_METER,
			blockWidth = Globals::BLOCK_WIDTH / Globals::PIXELS_PER_METER;

		b2Body* body = createStaticEntityBody(position, blockWidth, blockHeight);
		body->SetUserData((void*)EntityIndexes::Block);
		b2Fixture* fixture = body->GetFixtureList();
		while (fixture != nullptr) {
			fixture->SetUserData((void*)EntityIndexes::Block);
			fixture->SetFriction(1.0);
			fixture = fixture->GetNext();
		}

		GameEngine::RenderComponent rc(Globals::MODELS_LOCATION + "Block/" + modelName + "__001.png", b2Vec2(Globals::BLOCK_WIDTH, Globals::BLOCK_HEIGHT), body);
		Block* block = new Block(rc);

		entityManager_.addToWorld(block);

		return block;
	}

	GameEntity* EntityFactory::createFromName(b2Vec2 position, std::string name)
	{
		GameEntity *result = nullptr;
		if (name.compare("grass") == 0)
		{
			result = createBlock(position, "Ground");
		}

		return result;
	}

	EntityComponents EntityFactory::createEntityComponents(EntityDescriptor descriptor, int animationSpeed)
	{
		b2Body* body = createDynamicEntityBody(
			descriptor.position,
			Globals::DEFAULT_ENTITY_WIDTH / Globals::PIXELS_PER_METER,
			Globals::DEFAULT_ENTITY_HEIGHT / Globals::PIXELS_PER_METER);
		
		GameEngine::RenderComponent* rc = new GameEngine::RenderComponent(
			Globals::MODELS_LOCATION + descriptor.entityName + "/" + descriptor.modelName + "__001.png",
			b2Vec2(Globals::DEFAULT_ENTITY_WIDTH, Globals::DEFAULT_ENTITY_HEIGHT),
			body);
		GameEngine::AnimationComponent* ac = new GameEngine::AnimationComponent(
			Globals::MODELS_LOCATION + descriptor.entityName,
			descriptor.animation,
			animationSpeed, //default animation speed
			rc->getTextureName());

		EntityComponents components;
		components.animationComponent = ac;
		components.renderComponent = rc;
		components.body = body;

		return components;
	}

	Enemy* EntityFactory::createSkeleton(b2Vec2 position, std::string modelName, float scanRange, float damage, float range, float haste)
	{
		EntityDescriptor descriptor;
		descriptor.animation = modelName;
		descriptor.entityName = "Skeleton";
		descriptor.modelName = modelName;
		descriptor.position = position;

		EntityComponents components = createEntityComponents(descriptor, 80);

		Enemy *skeleton = new Skeleton(
			*components.renderComponent,
			*components.animationComponent,
			Globals::ENTITY_JUMP_POWER,
			scanRange,
			damage,
			range,
			haste);

		entityManager_.addToWorld(skeleton);

		return skeleton;
	}

	Tree* EntityFactory::createTree(b2Vec2 position, std::string modelName)
	{
		float treeHeight = Globals::TREE_HEIGHT / Globals::PIXELS_PER_METER,
			treeWidth = Globals::TREE_WIDTH / Globals::PIXELS_PER_METER;

		b2Body* body = createStaticEntityBody(position, treeWidth, treeHeight);
		GameEngine::RenderComponent rc(Globals::MODELS_LOCATION + "Tree/" + modelName + "__001.png", b2Vec2(Globals::TREE_WIDTH, Globals::TREE_HEIGHT), body);
		Tree* tree = new Tree(rc);

		entityManager_.addToWorld(tree);

		return tree;
	}

	EntityFactory::~EntityFactory()
	{
	}
}
