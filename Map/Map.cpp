#include "../BasicTypes/Entity.h"
#include "../WorldClient.h"
#include "../Entities/NPC.h"
#include "../Entities/Monster.h"
#include "../FileTypes/AIP.h"
#include "../FileTypes/STB.h"
#include "../FileTypes/ZON.h"
#include "Map.h"
#include "MapSector.h"
#include "MapRemovalRequest.h"
#include <iostream>
#include <thread>

Map::Map(const uint16_t id, const char* mapName, ZONFile* zoneFile, ZoneSTBFile* stbFile) {
	char buffer[0x10] = { 0x00 };
	sprintf_s(buffer, "Map-%i", id);
	logger.setLoggerName(buffer);
	this->id = id;
	this->mapName = mapName;
	mapTime = MapTime(stbFile, id);
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
	bool isInUse = false;
	bool emptyIdFound = false;
	uint16_t attempts = 10000;
	do {
		id = rand() % 0x10000;
		isInUse = localIds[id];
		emptyIdFound = id > 0 && !isInUse;
	} while (!emptyIdFound && (attempts--) > 0);

	if (emptyIdFound) {
		logger.logTrace("Found empty id for entity: ", id);
		localIds[id] = true;
		entity->getLocationData()->setLocalId(id);
	}
	return emptyIdFound;
}

void Map::clearLocalId(Entity* entity) {
	localIds[entity->getLocationData()->getLocalId()] = false;
	entity->getLocationData()->setLocalId(0);
}

void Map::addMonsterSpawn(std::shared_ptr<IFOMonsterSpawnEntry> spawnEntry) {
	MonsterRecoveryPoint* newSpawnPoint = new MonsterRecoveryPoint(spawnEntry, [this](Monster* newMonster) {
		if (!addEntityToInsertionQueue(newMonster)) {
			logger.logWarn("Failed to add monster to insertion queue.");
		}
	});
	monsterSpawnPoints.push_back(newSpawnPoint);
}

void Map::checkForMonsterRespawns() {
	for (auto monsterSpawn : monsterSpawnPoints) {
		monsterSpawn->checkForNewSpawns();
	}
}
void Map::updateMapTime() {
	mapTime.updateTime();
	mapTime.getCurrentDayTimeType();
}


Position Map::getDefaultRespawnPoint() {
	for (auto restorePoint : this->restorePoints) {
		if (_stricmp(restorePoint->getName(), RestorePoint::DEFAULT_RESTORE_POINT_NAME) == 0) {
			return restorePoint->getCenterPosition();
		}
	}
	logger.logWarn("No default respawn found for map: ", getName());
	return Position(520000.0f, 520000.0f);
}
void Map::addRestorePoints(std::vector<RestorePoint*> restorePoints) {
	logger.logTrace("Adding restore points...");
	for (auto point : restorePoints) {
		if(point->getMapId() == getId()) {
			this->restorePoints.push_back(point);
		}
	}
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
		else if (entity->isNPC() || entity->isMonster()) {
			dynamic_cast<NPC*>(entity)->updateAiProcessor(AIEvent::SPAWNED);
		}
		logger.logDebug("Adding entity ", entity->getLocationData()->getLocalId(), " to map.");
		entity->getLocationData()->setMap(this);
		entity->setIngame(true);
		entity->updateCombatValues();
	});
	entityInsertionQueue.clear();
}

bool Map::removeQueuedEntity(std::unordered_map<uint16_t, Entity*>::iterator& allEntityIteratorPosition) {
	bool removedFlag = entityRemovalQueue.find(allEntityIteratorPosition->first) != entityRemovalQueue.cend();
	if (removedFlag) {
		auto request = entityRemovalQueue.at(allEntityIteratorPosition->first);
		logger.logDebug("Removing entity with ID ", request->getLocalId(), " from map-queue ", getId());
		auto entity = allEntityIteratorPosition->second;
		if (entity->isPlayer()) {
			removePlayerFromRequest(request);
		}
		else if (entity->isNPC() || entity->isMonster()) {
			auto recoveryPoint = dynamic_cast<NPC*>(entity)->getMonsterRecoveryPoint();
			if (recoveryPoint != nullptr) {
				recoveryPoint->removeFromActivelySpawned(entity->getLocationData()->getLocalId());
			}
		}
		request->getMapSector()->removeEntity(entity);

		std::lock_guard<std::mutex> lock(entityRemovalMutex);
		allEntityIteratorPosition = allEntities.erase(allEntityIteratorPosition);
		clearLocalId(entity);
		entityRemovalQueue.erase(request->getLocalId());
	}
	return removedFlag;
}

bool Map::removePlayerFromRequest(std::shared_ptr<RemovalRequest>& request) {
	auto surroundingSectors = findSurroundingSectors(request->getMapSector());
	std::for_each(surroundingSectors.begin(), surroundingSectors.end(), [request](const std::pair<uint32_t, MapSector*>& pair) {
		pair.second->removeDisconnectingPlayer(request->getLocalId());
	});
	allPlayer.erase(request->getLocalId());
	return true;
}

bool Map::addEntityToInsertionQueue(Entity* entity) {
	bool success = assignNewLocalId(entity);
	if (success) {
		std::lock_guard<std::mutex> lock(entityInsertionMutex);
		entityInsertionQueue.insert(entityInsertionQueue.end(), entity);
		logger.logTrace("Added entity with id '", entity->getLocationData()->getLocalId(), "' to insertion queue.");
	} else {
		logger.logError("Entity could not be added to insertion queue.");
	}
	return success;
}

void Map::addEntityToRemovalQueue(Entity* entity, RemovalReason reason) {
	if (entity == nullptr || !entity->isIngame() || entity->getLocationData()->getCurrentMapSector() == nullptr || entity->getLocationData()->getLocalId() == 0x00) {
		return;
	}
	entity->getCombat()->clearSelfFromTargetsCombat();
	entity->getCombat()->clear();
	entity->setIngame(false);
	std::shared_ptr<RemovalRequest> removalRequest = RemovalRequestFactory::createRemovalRequest(entity, reason);
	std::lock_guard<std::mutex> lock(entityRemovalMutex);
	entityRemovalQueue.insert(std::move(std::make_pair(removalRequest->getLocalId(), removalRequest)));
	logger.logTrace("Added entity with id ", entity->getLocationData()->getLocalId(), " to removal queue.");
}

void Map::removeAllQueuedEntities() {
	std::lock_guard<std::mutex> lock(entityRemovalMutex);
	std::for_each(entityRemovalQueue.begin(), entityRemovalQueue.end(), [this](const std::pair<uint16_t, std::shared_ptr<RemovalRequest>>& pair) {
		uint16_t localId = pair.first;
		std::shared_ptr<RemovalRequest> request = pair.second;
		auto entity = request->getMapSector()->getEntity(localId);
		if (entity->isPlayer()) {
			removePlayerFromRequest(request);
		}
		clearLocalId(entity);
		entityRemovalQueue.erase(request->getLocalId());
	});
}

void Map::updateEntities() {
	addQueuedEntities();
	if (!isActive()) {
		return;
	}
	for (auto it = allEntities.begin(); it != allEntities.end();) {
		if (removeQueuedEntity(it)) {
			continue;
		}
		const std::pair<uint16_t, Entity*>& pair = (*it);
		auto entity = pair.second;
		PositionUpdateResult result = entity->updateMovement();
		if (entity->getVisualityProcessor()->isVisualityUpdateRequired()) {
			entity->updateVisuality();
		}
		switch (result) {
			case PositionUpdateResult::IDLE:
				entity->onIdle();
			break;
			case PositionUpdateResult::IS_MOVING:
				entity->onMoving();
			break;
			case PositionUpdateResult::TARGET_REACHED:
				if (entity->getCombat()->getTarget() != nullptr && !entity->getCombat()->isAttackRunning()) {
					entity->getCombat()->onTargetReached();
				}
				entity->getCombat()->refresh();
			break;
		}
		entity->onUpdate();
		it++;
	}
	checkForMonsterRespawns();
	updateMapTime();
}

Entity* Map::findEntityByLocalId(const uint16_t id) const {
	Entity* foundEntity = nullptr;
	std::for_each(mapSectors.begin(), mapSectors.end(), [&id, &foundEntity](const std::pair<uint32_t, MapSector*>& pair) {
		Entity *currentEntity = pair.second->getEntity(id);
		if (currentEntity) {
			foundEntity = currentEntity;
			return false;
		}
		return true;
	});
	return foundEntity;
}

bool Map::isSectorOutdatedForEntity(Entity* entity) const {
	auto locationData = entity->getLocationData();
	auto currentPosition = locationData->getMapPosition()->getCurrentPosition();
	auto bestSector = findBestSector(currentPosition);
	return bestSector != locationData->getCurrentMapSector();
}

MapSector* Map::findBestSector(Entity* entity) const {
	auto positionCollection = entity->getLocationData()->getMapPosition();
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

MapTime::MapTime() {
	totalDayTimeInSeconds = morningStartTimeInSeconds = noonStartTimeInSeconds = 0;
	eveningStartTimeInSeconds = nightStartTimeInSeconds = 0;
}

MapTime::MapTime(ZoneSTBFile* file, uint16_t mapId) {
	totalDayTimeInSeconds = file->getTotalDayTimeInSeconds(mapId) * 10;
	mapTime.setDurationForWrappingInMillis(totalDayTimeInSeconds * 1000);

	morningStartTimeInSeconds = file->getMorningStartTimeInSeconds(mapId) * 10;
	noonStartTimeInSeconds = file->getNoonStartTimeInSeconds(mapId) * 10;
	eveningStartTimeInSeconds = file->getEveningStartTimeInSeconds(mapId) * 10;
	nightStartTimeInSeconds = file->getNightStartTimeInSeconds(mapId) * 10;
}

MapTimeType MapTime::getCurrentDayTimeType() const {
	uint64_t currentTimeInSeconds = getCurrentTimeInSeconds();
	if (currentTimeInSeconds >= nightStartTimeInSeconds) {
		return MapTimeType::NIGHT_TIME;
	}
	else if (currentTimeInSeconds >= eveningStartTimeInSeconds) {
		return MapTimeType::EVENING_TIME;
	}
	else if (currentTimeInSeconds >= noonStartTimeInSeconds) {
		return MapTimeType::NOON_TIME;
	}
	return MapTimeType::MORNING_TIME;
}