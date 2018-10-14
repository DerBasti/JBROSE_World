#ifndef __ROSE_TELEGATE__
#define __ROSE_TELEGATE__

#pragma once
#include "..\BasicTypes\Position.h"
#include "..\FileTypes\ZON.h"

class Telegate {
private:
	Position position;
	uint32_t mapId;
public:
	Telegate(EventZoneData* telegateZoneData, uint32_t mapId) {
		position = telegateZoneData->getCenterPosition();
		this->mapId = mapId;
	}

	__inline const Position& getPosition() const {
		return position;
	}
	__inline uint32_t getMapId() const {
		return mapId;
	}
};

#endif //__ROSE_TELEGATE__