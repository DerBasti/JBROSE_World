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
			return;
		}
		success &= otherEntity->spawnVisually(entity);
		success &= entity->spawnVisually(otherEntity);
	});
	return success;
}

bool MapSector::despawnEntityVisually(Entity* entity) {
	bool success = true;
	std::for_each(entitiesInSector.begin(), entitiesInSector.end(), [&success, &entity](std::pair<uint16_t, Entity*> pair) {
		auto otherEntity = pair.second;
		if (otherEntity == entity || !entity->isIngame() || !otherEntity->isIngame()) {
			return;
		}
		success &= otherEntity->despawnVisually(entity->getLocationData()->getLocalId());
		success &= entity->despawnVisually(otherEntity->getLocationData()->getLocalId());
	});
	return success;
}

bool MapSector::sendDataToAllPlayer(const ResponsePacket& packet) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &packet](std::pair<uint16_t, Player*> pair) {
		auto player = pair.second;
		if (player->isIngame()) {
			success &= player->sendDataToSelf(packet);
		}
	});
	return success;
}

bool MapSector::sendDataToAllPlayerExcept(const ResponsePacket& packet, Player* player) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &packet, &player](std::pair<uint16_t, Player*> pair) {
		auto otherPlayer = pair.second;
		if (otherPlayer == player || player == nullptr || !player->isIngame()) {
			return;
		}
		success &= otherPlayer->sendDataToSelf(packet);
	});
	return success;
}

bool MapSector::despawnDisconnectingPlayerVisually(uint16_t localId) {
	bool success = true;
	std::for_each(playerInSector.begin(), playerInSector.end(), [&success, &localId](std::pair<uint16_t, Player*> pair) {
		auto player = pair.second;
		if (player == nullptr || !player->isIngame()) {
			return;
		}
		success &= player->despawnVisually(localId);
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

void MapSector::removeEntity(Entity* entity) {
	uint16_t localId = entity->getLocationData()->getLocalId();
	entitiesInSector.erase(localId);
	playerInSector.erase(localId);
}

void MapSector::removeDisconnectingPlayer(uint16_t entityLocalId) {
	entitiesInSector.erase(entityLocalId);
	playerInSector.erase(entityLocalId);
	despawnDisconnectingPlayerVisually(entityLocalId);
}