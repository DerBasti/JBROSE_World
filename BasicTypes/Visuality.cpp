#include "Visuality.h"
#include "Entity.h"
#include "..\Entities\Monster.h"
#include "..\WorldClient.h"
#include "../Entities/Drop.h"
#include "..\WorldPackets\Responses\SpawnDropVisuallyResponsePacket.h"
#include "..\WorldPackets\Responses\SpawnEntityVisuallyResponsePacket.h"
#include "..\WorldPackets\Responses\SpawnPlayerVisuallyResponsePacket.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"
#include "..\WorldPackets\Responses\SpawnNPCVisuallyResponsePacket.h"
#include "..\Map\Map.h"
#include "..\Map\MapSector.h"
#include <iostream>

VisualityProcessor::VisualityProcessor(Entity* entity) {
	this->entity = entity; 
	updateRequired = true;
}

VisualityProcessor::~VisualityProcessor() {
	entity = nullptr;
}

std::shared_ptr<SpawnEntityVisuallyResponsePacket> VisualityProcessor::createSpawnVisuallyPacket() {
	return std::shared_ptr<SpawnEntityVisuallyResponsePacket>();
}


bool VisualityProcessor::updateVisuality() {
	auto locationData = getEntity()->getLocationData();
	Map *map = locationData->getMap();
	MapSector* newSector = locationData->getCurrentMapSector();
	auto surroundingSectors = map->findSurroundingSectors(newSector);
	
	for(auto it=visibleSectors.cbegin();it != visibleSectors.cend();) {
		auto currentSector = it->second;
		if (surroundingSectors.find(currentSector->getId()) == surroundingSectors.cend()) {
			//std::cout << "Sector[" << currentSector->getId() << "] will now be despawned from Visuality.\n";
			it = visibleSectors.erase(it);
			currentSector->despawnEntityVisually(getEntity());
		}
		else {
			surroundingSectors.erase(currentSector->getId());
			++it;
		}
	}
	std::for_each(surroundingSectors.cbegin(), surroundingSectors.cend(), [this, map](std::pair<uint32_t, MapSector*> pair) {
		auto currentSector = pair.second;
		if (visibleSectors.find(currentSector->getId()) == visibleSectors.cend()) {
			//std::cout << "Visible sector[" << currentSector->getId() << "] will now be spawned to Visuality.\n";
			visibleSectors.insert(std::make_pair(currentSector->getId(), currentSector));
			currentSector->spawnEntityVisually(getEntity());
		}
	});
	setVisualityUpdateRequired(false);
	return true;
}

Entity* VisualityProcessor::findEntity(const uint16_t localId) {
	Entity* result = nullptr;
	std::find_if(visibleSectors.cbegin(), visibleSectors.cend(), [&localId, &result](std::pair<uint32_t, MapSector*> pair) {
		auto currentSector = pair.second;
		Entity* foundEntity = currentSector->getEntity(localId);
		result = foundEntity;
		return foundEntity != nullptr;
	});
	return result;
}

/*
============== MONSTER ===============
*/

MonsterVisualityProcessor::MonsterVisualityProcessor(Entity* entity) : VisualityProcessor(entity) {
}

MonsterVisualityProcessor::~MonsterVisualityProcessor() {

}

std::shared_ptr<SpawnEntityVisuallyResponsePacket> MonsterVisualityProcessor::createSpawnVisuallyPacket() {
	return std::shared_ptr<SpawnMonsterVisuallyResponsePacket>(new SpawnMonsterVisuallyResponsePacket(dynamic_cast<Monster*>(getEntity())));
}

/*
============== NPC ===============
*/

NPCVisualityProcessor::NPCVisualityProcessor(Entity* entity) : VisualityProcessor(entity) {
}

NPCVisualityProcessor::~NPCVisualityProcessor() {

}

std::shared_ptr<SpawnEntityVisuallyResponsePacket> NPCVisualityProcessor::createSpawnVisuallyPacket() {
	return std::shared_ptr<SpawnNPCVisuallyResponsePacket>(new SpawnNPCVisuallyResponsePacket(dynamic_cast<NPC*>(getEntity())));
}

/*
============== PLAYER ===============
*/

PlayerVisualityProcessor::PlayerVisualityProcessor(Entity* entity) : VisualityProcessor(entity) {
}

PlayerVisualityProcessor::~PlayerVisualityProcessor() {

}

std::shared_ptr<SpawnEntityVisuallyResponsePacket> PlayerVisualityProcessor::createSpawnVisuallyPacket() {
	return std::shared_ptr<SpawnPlayerVisuallyResponsePacket>(new SpawnPlayerVisuallyResponsePacket(dynamic_cast<Player*>(getEntity())));
}

/*
============== Drop ===============
*/

DropVisualityProcessor::DropVisualityProcessor(Entity* entity) : VisualityProcessor(entity) {
}

DropVisualityProcessor::~DropVisualityProcessor() {

}

std::shared_ptr<SpawnEntityVisuallyResponsePacket> DropVisualityProcessor::createSpawnVisuallyPacket() {
	return std::shared_ptr<SpawnDropVisuallyResponsePacket>(new SpawnDropVisuallyResponsePacket(dynamic_cast<Drop*>(getEntity())));
}