#include "MapSector.h"
#include "..\BasicTypes\Entity.h"
#include "..\WorldClient.h"
#include "..\..\JBROSE_Common\Packet.h"
#include <algorithm>

MapSector::MapSector(const uint32_t id, const Position& center) {
	this->id = id;
	this->center = center;
}

MapSector::~MapSector() {

}

bool MapSector::spawnEntityVisually(Entity* entity) {
	bool success = true;
	std::for_each(entitiesInSector.begin(), entitiesInSector.end(), [&success, &entity](std::pair<uint16_t, Entity*> pair) {
		auto otherEntity = pair.second;
		if (otherEntity == entity || !entity->isIngame() || !otherEntity->isIngame()) {
			return true;
		}
		success &= otherEntity->spawnVisually(entity);
		success &= entity->spawnVisually(otherEntity);
		return true;
	});
	return success;
}

bool MapSector::despawnEntityVisually(Entity* entity) {
	bool success = true;
	std::for_each(entitiesInSector.begin(), entitiesInSector.end(), [&success, &entity](std::pair<uint16_t, Entity*> pair) {
		auto otherEntity = pair.second;
		if (otherEntity == entity || !entity->isIngame() || !otherEntity->isIngame()) {
			return true;
		}
		if (otherEntity->getCombat()->getTarget() == entity) {
			otherEntity->getCombat()->clear();
		}
		success &= otherEntity->despawnVisually(entity->getLocationData()->getLocalId());
		success &= entity->despawnVisually(otherEntity->getLocationData()->getLocalId());
		return true;
	});
	return success;
}

bool MapSector::sendDataToAllPlayer(const std::shared_ptr<ResponsePacket>& packet) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &packet](std::pair<uint16_t, Player*> pair) {
		auto player = pair.second;
		if (player != nullptr && player->isIngame()) {
			success &= player->getPacketHandler()->sendDataToClient(*packet.get());
		}
	});
	return success;
}

bool MapSector::sendDataToAllPlayerExcept(const std::shared_ptr<ResponsePacket>& packet, const Player* player) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &packet, &player](std::pair<uint16_t, Player*> pair) {
		auto otherPlayer = pair.second;
		if (otherPlayer == player || player == nullptr || !player->isIngame()) {
			return true;
		}
		success &= player->getPacketHandler()->sendDataToClient(*packet.get());
		return true;
	});
	return success;
}

void MapSector::addPacketToDistributionQueue(const std::shared_ptr<ResponsePacket>& packet) {
	addPacketToDistributionQueue(packet, nullptr);
}

void MapSector::addPacketToDistributionQueue(const std::shared_ptr<ResponsePacket>& packet, const Player* playerNotToSendPacketTo) {
	std::lock_guard<std::mutex> guard(packetQueueMutex);
	this->visualityChangePacketQueue.push(std::move(DistributionPacket(packet, playerNotToSendPacketTo)));
}

void MapSector::distributeVisualityChangePackets() {
	std::lock_guard<std::mutex> guard(packetQueueMutex);
	while (!visualityChangePacketQueue.empty()) {
		auto distributionPacket = visualityChangePacketQueue.front();
		if (distributionPacket.getPlayerNotToSendPacketTo() != nullptr) {
			sendDataToAllPlayerExcept(distributionPacket.getResponsePacket(), distributionPacket.getPlayerNotToSendPacketTo());
		}
		else {
			sendDataToAllPlayer(distributionPacket.getResponsePacket());
		}
		visualityChangePacketQueue.pop();
	}
}

bool MapSector::despawnDisconnectingPlayerVisually(uint16_t localId) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &localId](std::pair<uint16_t, Player*> pair) {
		auto player = pair.second;
		if (player == nullptr || !player->isIngame()) {
			return true;
		}
		success &= player->despawnVisually(localId);
		return true;
	});
	return success;
}

bool MapSector::addEntity(Entity* entity) {
	entitiesInSector.insert(std::make_pair(entity->getLocationData()->getLocalId(), entity));
	if (entity->isPlayer()) {
		playerInSector.insert(std::make_pair(entity->getLocationData()->getLocalId(), dynamic_cast<Player*>(entity)));
	}
	return true;
}

bool MapSector::hasEntity(const uint16_t id) {
	bool success = getEntity(id) != nullptr;
	return success;
}

Entity* MapSector::getEntity(const uint16_t id) {
	Entity* entity = nullptr;
	std::for_each(entitiesInSector.begin(), entitiesInSector.end(), [&entity, &id](std::pair<uint16_t, Entity*> pair) {
		auto otherEntity = pair.second;
		if (otherEntity->getLocationData()->getLocalId() == id) {
			entity = otherEntity;
		}
		return entity == nullptr;
	});
	return entity;
}

Entity* MapSector::findFirstEntity(std::function<bool(Entity*)> searchFunction) {
	Entity* entity = nullptr;
	std::for_each(entitiesInSector.begin(), entitiesInSector.end(), [&entity, searchFunction](std::pair<uint16_t, Entity*> pair) {
		Entity* currentEntity = pair.second;
		if (searchFunction(currentEntity)) {
			entity = currentEntity;
			return false;
		}
		return true;
	});
	return entity;
}

void MapSector::removeEntity(Entity* entity) {
	uint16_t localId = entity->getLocationData()->getLocalId();
	removeEntity(localId);
}

void MapSector::removeEntity(uint16_t localId) {
	entitiesInSector.erase(localId);
	playerInSector.erase(localId);
}

void MapSector::removeDisconnectingPlayer(uint16_t entityLocalId) {
	entitiesInSector.erase(entityLocalId);
	playerInSector.erase(entityLocalId);
	despawnDisconnectingPlayerVisually(entityLocalId);
}