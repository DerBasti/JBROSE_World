#ifndef __ROSE_LOCATION_DATA__
#define __ROSE_LOCATION_DATA__
#pragma once

#include "Position.h"
#include <inttypes.h>
#include <memory>
#include <iostream>

class LocationData {
private:
	class Map* map;
	class MapSector* currentSector;
	MapPosition* mapPosition;
	uint16_t localId;
public:
	LocationData();
	virtual ~LocationData();

	Map* getMap() const;
	void setMap(Map* map);

	MapSector* getCurrentMapSector() const;
	void setCurrentMapSector(MapSector* sector);


	__inline MapPosition* getMapPosition() const {
		return mapPosition;
	}
	__inline void setMapPosition(MapPosition* positionCollection) {
		this->mapPosition = positionCollection;
	}

	__inline uint16_t getLocalId() const {
		return localId;
	}
	__inline void setLocalId(const uint16_t localId) {
		this->localId = localId;
	}
};


std::ostream& operator<<(std::ostream& out, const LocationData* locationData);
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<LocationData>& locationData);
std::wostream& operator<<(std::wostream& out, const LocationData* locationData);
std::wostream& operator<<(std::wostream& out, const std::shared_ptr<LocationData>& locationData);

#endif //__ROSE_LOCATION_DATA__