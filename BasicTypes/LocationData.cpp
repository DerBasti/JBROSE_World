#include "LocationData.h"
#include "..\Map\Map.h"


LocationData::LocationData() {
	positionCollection = new PositionCollection(Position(520000.0f, 520000.0f));
}

LocationData::~LocationData() {
	delete positionCollection;
	positionCollection = nullptr;
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

std::ostream& operator<<(std::ostream& out, const LocationData* locationData) {
	out << "[MapId: " << locationData->getMap()->getId() << ", Position: " << locationData->getPositionCollection()->getCurrentPosition() << "]";
	return out;
}