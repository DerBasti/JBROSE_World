#include "LocationData.h"
#include "..\Map\Map.h"


LocationData::LocationData() {
	mapPosition = new MapPosition(Position(520000.0f, 520000.0f));
	map = nullptr;
	currentSector = nullptr;
	localId = 0;
}

LocationData::~LocationData() {
	delete mapPosition;
	mapPosition = nullptr;

	map = nullptr;
	currentSector = nullptr;
	localId = 0;
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
	out << "[MapId: " << locationData->getMap()->getId() << ", Position: " << locationData->getMapPosition()->getCurrentPosition() << "]";
	return out;
}

std::ostream& operator<<(std::ostream& out, const std::shared_ptr<LocationData>& locationData) {
	return operator<<(out, locationData.get());
}