#include "Entity.h"
#include "..\Map\Map.h"
#include "..\Map\MapSector.h"
#include "../WorldPackets/ResponsePackets.h"
#include "../FileTypes/QSD.h"
#include "../FileTypes/ZMO.h"
#include "../WorldClient.h"
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
		std::shared_ptr<ResponsePacket> response = std::shared_ptr<ResponsePacket>(new NewDestinationResponsePacket(this));
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
	if (result == PositionUpdateResult::IDLE && !positionProcessor->isUpdateRequired()) {
		return result;
	}
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
	positionProcessor->setUpdateRequiredFlag(false);
	return result;
}

const char* Entity::getName() const {
	return "";
}

bool Entity::doAttack() {
	if (getCombat()->hasTarget()) {
		auto hit = getCombat()->doBasicAttack();

		std::shared_ptr<BasicAttackResponsePacket> packet = std::shared_ptr<BasicAttackResponsePacket>(new BasicAttackResponsePacket(hit));
		if (hit.isDeadlyHit()) {
			return this->sendDataToVisibleEntities(packet);
		}
		else {
			sendDataToEntityMap(getCombat()->getTarget()->getCombat()->getTargetedByEnemiesList(), packet);
			getCombat()->getTarget()->sendDataToSelf(*packet.get());
		}
	}
	return true;
}

bool Entity::onNewTarget() {
	std::shared_ptr<InitBasicAttackResponsePacket> packet = std::shared_ptr<InitBasicAttackResponsePacket>(new InitBasicAttackResponsePacket(getCombat()));
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
	std::shared_ptr<StanceResponsePacket> packet = std::shared_ptr<StanceResponsePacket>(new StanceResponsePacket());

	packet->setLocalEntityId(getLocationData()->getLocalId());
	packet->setMovementSpeed(getStats()->getMovementSpeed());
	packet->setStanceType(getStance()->getStanceId());

	return sendDataToVisibleEntities(packet);
}

bool EntityComparator::operator()(const Entity* left, const Entity* right) const {
	return left->getLocationData()->getLocalId() == right->getLocationData()->getLocalId();
}

bool Entity::sendDataToVisibleEntities(const std::shared_ptr<ResponsePacket>& packet) const {
	bool success = true;
	getLocationData()->getMap()->sendDataToAllVisiblePlayerOfEntity(this, packet);
	return success;
}

bool Entity::sendDataToEntityMap(const std::map<uint16_t, Entity*>& entityMap, const std::shared_ptr<ResponsePacket>& packet) const {
	bool success = true;
	std::for_each(entityMap.begin(), entityMap.end(), [&packet, &success](const std::pair<uint16_t, Entity*>& pair) {
		success &= pair.second->sendDataToSelf(*packet.get());
	});
	return success;
}

bool Entity::sendDataToVisibleExceptSelf(const std::shared_ptr<ResponsePacket>& packet) const {
	return isPlayer() && getLocationData()->getMap()->sendDataToAllVisiblePlayerOfEntityExcept(this, packet, dynamic_cast<const Player*>(this));
}

bool Entity::handleQuestTriggerRun(std::shared_ptr<QuestRecord> initialRecord) {
	if (!initialRecord) {
		return false;
	}
	std::shared_ptr<QuestRecord> currentRecord = initialRecord;
	QuestTriggerContext context(this);
	do {
		if (!currentRecord->conditionsFulfilled(context)) {
			if (!currentRecord->hasToCheckNextRecord()) {
				return false;
			}
			continue;
		}
		currentRecord->handleRewards(context);

		if (currentRecord != initialRecord && isPlayer()) {
			QuestJournalUpdateResponsePacket response(QuestJournalUpdateResult::TRIGGER_RUN_SUCCESSFUL);
			response.setQuestHash(currentRecord->getQuestHash());
			response.setQuestJournalSlot(context.getSelectedQuest()->getQuestJournalSlot());
			dynamic_cast<Player*>(this)->sendDataToSelf(response);
		}

	} while (currentRecord = currentRecord->getNextRecord());
	return true;
}