#include "LocationData.h"
#include "..\Map\Map.h"


LocationData::LocationData() {
	positionCollection = std::shared_ptr<PositionCollection>(new PositionCollection(Position(520000.0f, 520000.0f)));
}

LocationData::~LocationData() {
}

Map* LocationData::getMap() const {
	return map;
}

void LocationData::setMap(Map* map) {
	this->map = map;
}

MapSector* LocationData::getCurrentMapSector() const {
	return currentSector;
}

void LocationData::setCurrentMapSector(MapSector* sector) {
	currentSector = sector;
}