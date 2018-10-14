#ifndef __ROSE_MAP__
#define __ROSE_MAP__
#pragma once

#include "..\BasicTypes\Position.h"
#include <unordered_set>
#include <mutex>
#include <deque>
#include <unordered_map>
#include "MapRemovalRequest.h"

class Map {
private:
	const static uint8_t SECTORS_PER_AXIS = 40;
	constexpr static float SECTORS_START_COORDINATE = 384000.0f;
	constexpr static float SECTORS_SIZE = 6000.0f;
	constexpr static float SECTOR_DISTANCE_NECESSARY = SECTORS_SIZE * 1.1f;
	constexpr static int16_t surroundingSectors[] = {
		(-SECTORS_PER_AXIS) - 1, (-SECTORS_PER_AXIS), (-SECTORS_PER_AXIS)+1,
		-1, 0, 1,
		SECTORS_PER_AXIS - 1, SECTORS_PER_AXIS, SECTORS_PER_AXIS + 1
	};

	uint16_t id;
	std::mutex entityInsertionMutex;
	std::mutex entityRemovalMutex;
	std::unordered_map<uint32_t, class MapSector*> mapSectors;
	std::unordered_map<uint16_t, class Entity*> allEntities;
	std::unordered_map<uint16_t, class Player*> allPlayer;

	bool localIds[0x10000] = { false };

	std::deque<class Entity*> entityInsertionQueue;
	std::unordered_map<uint16_t, std::shared_ptr<class RemovalRequest>> entityRemovalQueue;

	void addQueuedEntities();
	bool removeQueuedEntity(std::unordered_map<uint16_t, class Entity*>::const_iterator& allEntityIteratorPosition);
	bool assignNewLocalId(Entity* entity);
	void clearLocalId(Entity* entity);
	bool removePlayerFromRequest(std::shared_ptr<class RemovalRequest>& request);

	MapSector* findBestSector(const Position& position) const;
public:
	Map(const uint16_t id);
	virtual ~Map();

	bool addEntityToInsertionQueue(Entity* entity);
	void updateEntities();
	void addEntityToRemovalQueue(Entity* entity, RemovalReason reason);
	bool isSectorOutdatedForEntity(Entity* entity) const;
	MapSector* findBestSector(Entity* entity) const;
	std::unordered_map<uint32_t, class MapSector*> findSurroundingSectors(class MapSector* sector);

	__inline uint16_t getId() const {
		return id;
	}
	__inline bool isActive() const {
		return !allPlayer.empty();
	}
};

#endif //__ROSE_MAP__