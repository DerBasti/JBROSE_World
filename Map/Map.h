#ifndef __ROSE_MAP__
#define __ROSE_MAP__
#pragma once

#include "..\BasicTypes\Position.h"
#include "..\..\JBROSE_Common\Logger.h"
#include <unordered_set>
#include <mutex>
#include <deque>
#include <unordered_map>
#include "MapRemovalRequest.h"
#include "MonsterRecoveryPoint.h"
#include "../../JBROSE_Common/Timer.h"
#include "MapTime.h"

class Map {
private:
	const static uint8_t SECTORS_PER_AXIS = 40;
	constexpr static float SECTORS_START_COORDINATE = 384000.0f;
	constexpr static float SECTORS_SIZE = 6000.0f;
	constexpr static float SECTOR_DISTANCE_NECESSARY = SECTORS_SIZE * 1.05f;
	constexpr static int16_t surroundingSectors[] = {
		(-SECTORS_PER_AXIS) - 1, (-SECTORS_PER_AXIS), (-SECTORS_PER_AXIS)+1,
		-1, 0, 1,
		SECTORS_PER_AXIS - 1, SECTORS_PER_AXIS, SECTORS_PER_AXIS + 1
	};

	ROSEThreadedLogger logger;
	uint16_t id;
	std::mutex entityInsertionMutex;
	std::mutex entityRemovalMutex;
	const char* mapName;
	MapTime mapTime;
	std::unordered_map<uint32_t, class MapSector*> mapSectors;
	std::unordered_map<uint16_t, class Entity*> allEntities;
	std::unordered_map<uint16_t, class Player*> allPlayer;
	std::vector<MonsterRecoveryPoint*> monsterSpawnPoints;
	std::vector<class RestorePoint*> restorePoints;
	std::vector<class IFOSpawn*> spawnPoints;

	bool localIds[0x10000] = { false };
	NumericRandomizer<uint16_t> localIdRandomizer;

	std::deque<class Entity*> entityInsertionQueue;
	std::unordered_map<uint16_t, std::shared_ptr<class RemovalRequest>> entityRemovalQueue;

	void addQueuedEntities();
	bool removeQueuedEntity(std::unordered_map<uint16_t, class Entity*>::iterator& allEntityIteratorPosition);
	bool assignNewLocalId(Entity* entity);
	void clearLocalId(Entity* entity);
	void clearLocalId(uint16_t localId);
	bool removePlayerFromRequest(std::shared_ptr<class RemovalRequest>& request);
	void removeAllQueuedEntities();
	void distributeVisualityChangePackets();

	MapSector* findBestSector(const Position& position) const;
public:
	Map(const uint16_t id, const char* mapName, class ZONFile* zoneFile, class ZoneSTBFile* zoneStb);
	virtual ~Map();

	bool addEntityToInsertionQueue(Entity* entity);
	void addMonsterSpawn(std::shared_ptr<IFOMonsterSpawnEntry> spawnEntry);
	void addRestorePoints(std::vector<RestorePoint*> restorePoints);
	void addEntityToRemovalQueue(Entity* entity, RemovalReason reason);

	bool sendDataToAllVisiblePlayerOfEntity(const Entity* entity, const std::shared_ptr<ResponsePacket>& packet) const;
	bool sendDataToAllVisiblePlayerOfEntityExcept(const Entity* entity, const std::shared_ptr<ResponsePacket>& packet, const class Player* playerNotToSendPacketTo) const;

	void updateEntities();
	void checkForMonsterRespawns();
	Entity* findEntityByLocalId(const uint16_t id) const;
	Position getDefaultRespawnPoint();
	void updateMapTime();

	bool isSectorOutdatedForEntity(Entity* entity) const;
	MapSector* findBestSector(Entity* entity) const;
	std::unordered_map<uint32_t, class MapSector*> findSurroundingSectors(class MapSector* sector);

	__inline uint16_t getId() const {
		return id;
	}
	__inline const char* getName() const {
		return mapName;
	}
	__inline bool isActive() const {
		return !allPlayer.empty();
	}
	__inline const MapTime& getMapTime() const {
		return mapTime;
	}
};

#endif //__ROSE_MAP__