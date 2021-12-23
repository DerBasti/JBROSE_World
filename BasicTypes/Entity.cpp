#include "Entity.h"
#include "..\Map\Map.h"
#include "..\Map\MapSector.h"
#include "../WorldPackets/ResponsePackets.h"
#include "../FileTypes/ZMO.h"
#include "Combat.h"
#include "Visuality.h"
#include <algorithm>
#include <iostream>

Entity::Entity() {
	stats = nullptr;
	locationData = new LocationData();
	combat = new Combat(this);

	positionProcessor = new PositionProcessor(getLocationData()->getMapPosition(), combat, [&]() {
		return getStats()->getMovementSpeed();
	});
	ingameFlag = false;

	auto lambda = [&]() {
		positionProcessor->onNewDestination();
		NewDestinationResponsePacket response(this);
		return sendDataToVisibleEntities(response);
	};
	getLocationData()->getMapPosition()->setUpdateDestinationVisualCallback(lambda);
}

Entity::~Entity() {
	logger.logDebug("Deconstructing Entity...");
	delete locationData;
	locationData = nullptr;

	delete combat;
	combat = nullptr;

	delete positionProcessor;
	positionProcessor = nullptr;

	ingameFlag = false;
}

PositionUpdateResult Entity::updateMovement() {
	PositionUpdateResult result = positionProcessor->processNewPosition();
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
	return result;
}

const char* Entity::getName() const {
	return "";
}

bool Entity::doAttack() {
	if (getCombat()->getTarget() != nullptr) {
		auto hit = getCombat()->doBasicAttack();

		BasicAttackResponsePacket packet(hit);
		if (hit.isDeadlyHit()) {
			return this->sendDataToVisibleEntities(packet);
		}
		else {
			sendDataToEntityMap(getCombat()->getTarget()->getCombat()->getTargetedByEnemiesList(), packet);
			getCombat()->getTarget()->sendDataToSelf(packet);
		}
	}
	return true;
}

bool Entity::onNewTarget() {
	InitBasicAttackResponsePacket packet(getCombat());
	bool success = this->sendDataToVisibleEntities(packet);
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

bool Entity::updateStanceVisually() {
	StanceResponsePacket packet;

	packet.setLocalEntityId(getLocationData()->getLocalId());
	packet.setMovementSpeed(getStats()->getMovementSpeed());
	packet.setStanceType(getStance()->getStanceId());

	return sendDataToVisibleEntities(packet);
}

bool EntityComparator::operator()(const Entity* left, const Entity* right) const {
	return left->getLocationData()->getLocalId() == right->getLocationData()->getLocalId();
}

bool Entity::sendDataToVisibleEntities(const ResponsePacket& packet) const {
	bool success = true;
	auto visibleSectors = getVisualityProcessor()->getVisibleSectors();
	std::for_each(visibleSectors.begin(), visibleSectors.end(), [&success, &packet](const std::pair<uint32_t, MapSector*>& pair) {
		auto sector = pair.second;
		success &= sector->sendDataToAllPlayer(packet);
	});
	return success;
}

bool Entity::sendDataToEntityMap(const std::map<uint16_t, Entity*>& entityMap, const ResponsePacket& packet) const {
	bool success = true;
	std::for_each(entityMap.begin(), entityMap.end(), [&packet, &success](const std::pair<uint16_t, Entity*>& pair) {
		success &= pair.second->sendDataToSelf(packet);
	});
	return success;
}

bool Entity::sendDataToVisibleExceptSelf(const ResponsePacket& packet) const {
	bool success = true;
	auto visibleSectors = getVisualityProcessor()->getVisibleSectors();
	std::for_each(visibleSectors.begin(), visibleSectors.end(), [&success, &packet](const std::pair<uint32_t, MapSector*>& pair) {
		auto sector = pair.second;
		success &= sector->sendDataToAllPlayerExcept(packet, nullptr);
	});
	return success;
}