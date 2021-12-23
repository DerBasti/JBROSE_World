#ifndef __ROSE_MAP_REMOVAL_REQUEST__
#define __ROSE_MAP_REMOVAL_REQUEST__

#pragma once
#include "MapSector.h"


class RemovalRequest {
private:
	uint16_t localId;
	MapSector* registeredSector;
public:
	RemovalRequest();
	RemovalRequest(const uint16_t localId, MapSector* sector);
	virtual ~RemovalRequest();

	__inline uint16_t getLocalId() const {
		return localId;
	}
	__inline MapSector* getMapSector() const {
		return registeredSector;
	}
};

enum class RemovalReason : uint8_t {
	TELEPORT, MONSTER_DEATH, PLAYER_DISCONNECT, DROP_PICKUP
};

class RemovalRequestFactory {
public:
	static std::shared_ptr<RemovalRequest> createRemovalRequest(class Entity* entity, RemovalReason reason);
};

#endif //__ROSE_MAP_REMOVAL_REQUEST__