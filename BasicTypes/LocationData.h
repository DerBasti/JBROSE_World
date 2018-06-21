#ifndef __ROSE_LOCATION_DATA__
#define __ROSE_LOCATION_DATA__
#pragma once

#include "Position.h"
#include <inttypes.h>
#include <memory>

class LocationData {
private:
	class Map* map;
	class MapSector* currentSector;
	std::shared_ptr<PositionCollection> positionCollection;
	uint16_t localId;
public:
	LocationData();
	virtual ~LocationData();

	Map* getMap() const;
	void setMap(Map* map);

	MapSector* getCurrentMapSector() const;
	void setCurrentMapSector(MapSector* sector);

	__inline std::shared_ptr<PositionCollection> getPositionCollection() const {
		return positionCollection;
	}
	__inline void setPositionCollection(const std::shared_ptr<PositionCollection>& positionCollection) {
		this->positionCollection = positionCollection;
	}

	__inline uint16_t getLocalId() const {
		return localId;
	}
	__inline void setLocalId(const uint16_t localId) {
		this->localId = localId;
	}
};

#endif //__ROSE_LOCATION_DATA__