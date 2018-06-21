#include "..\BasicTypes\Entity.h"
#include "..\WorldClient.h"
#include "Map.h"
#include "MapSector.h"
#include <iostream>
#include <thread>

Map::Map(const uint16_t id) {
	this->id = id;
	for (uint16_t i = 0; i < SECTORS_PER_AXIS*SECTORS_PER_AXIS; i++) {
		float x = SECTORS_START_COORDINATE + ((i % SECTORS_PER_AXIS) * SECTORS_SIZE);
		float y = SECTORS_START_COORDINATE + ((i / SECTORS_PER_AXIS) * SECTORS_SIZE);
		mapSectors.insert(std::make_pair(i, new MapSector(i, Position(x, y))));
	}
}

Map::~Map() {
	std::for_each(mapSectors.begin(), mapSectors.end(), [](std::pair<uint32_t, MapSector*> pair) {
		delete pair.second;
		pair.second = nullptr;
	});
}

std::unordered_map<uint32_t, MapSector*> Map::findSurroundingSectors(MapSector* sector) {
	std::unordered_map<uint32_t, MapSector*> resultMap;
	for (uint8_t i = 0; i < 9; i++) {
		uint16_t id = surroundingSectors[i] + sector->getId();
		resultMap.insert(std::make_pair(id, mapSectors.at(id)));
	}
	return resultMap;
}

bool Map::assignNewLocalId(Entity* entity) {
	uint16_t id = 0;
	Entity* foundEntity = nullptr;
	bool emptyIdFound = false;
	uint16_t attempts = 10000;
	do {
		id = rand() % 0x10000;
		foundEntity = localIds[id];
	} while (!(emptyIdFound = (id > 0 && foundEntity == nullptr)) && (attempts--) > 0);

	if (emptyIdFound) {
		localIds[id] = entity;
		entity->getLocationData()->setLocalId(id);
		entity->setIsIngame(true);
	}
	return emptyIdFound;
}

void Map::clearLocalId(Entity* entity) {
	localIds[entity->getLocationData()->getLocalId()] = nullptr;
	entity->getLocationData()->setLocalId(0);
	entity->setIsIngame(false);
}

void Map::addQueuedEntities() {
	if (entityInsertionQueue.empty()) {
		return;
	}
	std::lock_guard<std::mutex> lock(entityInsertionMutex);
	std::for_each(entityInsertionQueue.begin(), entityInsertionQueue.end(), [this](Entity* entity) {
		allEntities.insert(std::make_pair(entity->getLocationData()->getLocalId(), entity));
		if (entity->isPlayer()) {
			allPlayer.insert(std::make_pair(entity->getLocationData()->getLocalId(), dynamic_cast<Player*>(entity)));
		}
		entity->getLocationData()->setMap(this);
	});
	entityInsertionQueue.clear();
}

void Map::removeQueuedEntity(std::unordered_map<uint16_t, class Entity*>::const_iterator& allEntityIteratorPosition) {
	auto request = entityRemovalQueue.at(allEntityIteratorPosition->first);
	request->getMapSector()->removeDisconnectingPlayer(request->getLocalId());

	allEntityIteratorPosition = allEntities.erase(allEntityIteratorPosition);
	allPlayer.erase(request->getLocalId());
}

bool Map::addEntityToInsertionQueue(Entity* entity) {
	bool success = assignNewLocalId(entity);
	if (success) {
		std::lock_guard<std::mutex> lock(entityInsertionMutex);
		entityInsertionQueue.insert(entityInsertionQueue.end(), entity);
	} else {
		std::cout << "ENTITY COULD NOT BE ADDED TO THE MAP-INSERTIONQUEUE!\n";
	}
	return success;
}

void Map::addEntityToRemovalQueue(Entity* entity) {
	if (entity == nullptr || entity->getLocationData()->getCurrentMapSector() == nullptr || entity->getLocationData()->getLocalId() == 0x00) {
		return;
	}
	std::lock_guard<std::mutex> lock(entityRemovalMutex);
	std::shared_ptr<RemovalRequest> removalRequest(new RemovalRequest(entity->getLocationData()->getLocalId(), entity->getLocationData()->getCurrentMapSector()));
	clearLocalId(entity);
	entityRemovalQueue.insert(std::make_pair(removalRequest->getLocalId(), removalRequest));
}

void Map::updateEntities() {
	addQueuedEntities();
	if (!isActive()) {
		return;
	}
	for(auto it=allEntities.cbegin();it!=allEntities.cend();) {
		const std::pair<uint16_t, Entity*>& pair = (*it);
		entityRemovalMutex.lock();
		if (entityRemovalQueue.find(pair.first) != entityRemovalQueue.cend()) {
			removeQueuedEntity(it);
			entityRemovalMutex.unlock();
			continue;
		}
		entityRemovalMutex.unlock();
		auto entity = pair.second;
		entity->updateMovement();
		if (entity->getVisualityProcessor()->isVisualityUpdateRequired()) {
			entity->updateVisuality();
		}
		it++;
	}
}

bool Map::isSectorOutdatedForEntity(Entity* entity) const {
	auto locationData = entity->getLocationData();
	auto currentPosition = locationData->getPositionCollection()->getCurrentPosition();
	auto bestSector = findBestSector(currentPosition);
	return bestSector != locationData->getCurrentMapSector();
}

MapSector* Map::findBestSector(Entity* entity) const {
	auto positionCollection = entity->getLocationData()->getPositionCollection();
	MapSector* old = entity->getLocationData()->getCurrentMapSector();
	float distance = (old == nullptr ? SECTOR_DISTANCE_NECESSARY + 1.0f : PositionProcessor::getDistanceBetweenPoints(old->getCenterPosition(), positionCollection->getCurrentPosition()));
	MapSector* newBest = old;
	if (distance >= SECTOR_DISTANCE_NECESSARY) {
		newBest = findBestSector(positionCollection->getCurrentPosition());
	}
	return newBest;
}

MapSector* Map::findBestSector(const Position& position) const {
	uint16_t sectorX = static_cast<uint16_t>(std::roundf(static_cast<uint32_t>(position.getX() - SECTORS_START_COORDINATE) / SECTORS_SIZE));
	if (sectorX >= SECTORS_PER_AXIS) {
		sectorX = SECTORS_PER_AXIS - 1;
	}
	uint16_t sectorY = static_cast<uint16_t>(std::roundf(static_cast<uint32_t>(position.getY() - SECTORS_START_COORDINATE) / SECTORS_SIZE));
	if (sectorY >= SECTORS_PER_AXIS) {
		sectorY = SECTORS_PER_AXIS - 1;
	}

	uint16_t id = sectorX + (sectorY * SECTORS_PER_AXIS);
	return mapSectors.at(id);
}


Map::RemovalRequest::RemovalRequest() : RemovalRequest(0, nullptr) {}

Map::RemovalRequest::RemovalRequest(const uint16_t localId, MapSector* sector) {
	this->localId = localId;
	registeredSector = sector;
}
Map::RemovalRequest::~RemovalRequest() {
	registeredSector = nullptr;
	localId = 0;
}