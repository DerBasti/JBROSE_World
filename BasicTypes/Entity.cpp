#include "Entity.h"
#include "..\Map\Map.h"
#include "..\Map\MapSector.h"
#include "..\WorldPackets\Responses\SpawnEntityVisuallyResponsePacket.h"
#include "Visuality.h"
#include <iostream>

Entity::Entity() {
	locationData = new LocationData();
	positionProcessor = new PositionProcessor(getLocationData()->getPositionCollection());
	ingameFlag = false;
}

Entity::~Entity() {
	std::cout << "Deconstructing Entity...\n";
	delete locationData;
	locationData = nullptr;

	delete positionProcessor;
	positionProcessor = nullptr;

	ingameFlag = false;
}

bool Entity::updateMovement() {
	bool success = positionProcessor->processNewPosition();
	auto map = getLocationData()->getMap();
	auto oldSector = getLocationData()->getCurrentMapSector();
	auto newSector = map->findBestSector(this);
	if (oldSector != newSector) {
		if (oldSector != nullptr) {
			oldSector->removeEntity(this);
		}
		newSector->addEntity(this);
		getLocationData()->setCurrentMapSector(newSector);
		getVisualityProcessor()->setVisualityUpdateRequired(true);
	}
	return success;
}

bool Entity::updateVisuality() {
	bool success = visualityProcessor->updateVisuality();
	return success;
}

bool Entity::spawnVisually(Entity* entity) {
	return true;
}

bool Entity::despawnVisually(Entity* entity) {
	return true;
}

bool Entity::despawnVisually(uint16_t localId) {
	return true;
}

bool EntityComparator::operator()(const Entity* left, const Entity* right) const {
	return left->getLocationData()->getLocalId() == right->getLocationData()->getLocalId();
}