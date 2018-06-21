#ifndef __ROSE_MAP_SECTOR__
#define __ROSE_MAP_SECTOR__
#pragma once

#include <inttypes.h>
#include <unordered_map> 
#include "..\BasicTypes\Position.h"

class MapSector {
private:
	uint32_t id;
	std::unordered_map<uint16_t, class Entity*> entitiesInSector;
	std::unordered_map<uint16_t, class Player*> playerInSector;
	Position center;

	bool despawnDisconnectingPlayerVisually(uint16_t localId);
public:
	MapSector(const uint32_t id, const Position& center);
	virtual ~MapSector();

	bool addEntity(Entity* entity);
	void removeEntity(Entity* entity);
	void removeDisconnectingPlayer(uint16_t entityLocalId);

	bool spawnEntityVisually(class Entity* entity);
	bool despawnEntityVisually(class Entity* entity);

	bool sendDataToAllPlayer(const class ResponsePacket& packet);
	bool sendDataToAllPlayerExcept(const class ResponsePacket& packet, class Player* player);

	__inline uint32_t getId() const {
		return id;
	}
	__inline const Position& getCenterPosition() const {
		return center;
	}
};

#endif //__ROSE_MAP_SECTOR__