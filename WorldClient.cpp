#include "WorldClient.h"
#include "WorldPackets\RequestPackets.h"
#include "WorldPackets\ResponsePackets.h"
#include "WorldServer.h"
#include "Map/MapSector.h"
#include "..\JBROSE_Common\EncryptionHandler.h"
#include "Map/MapRemovalRequest.h"
#include <iostream>


Player::Player(std::shared_ptr<ROSEClient>& networkInterface) {
	networkConnection = networkInterface;
	stats = new PlayerStats();
	traits = new PlayerTraits();
	inventory = new Inventory();
	visualityProcessor = new PlayerVisualityProcessor(this);
}

Player::~Player() {
	networkConnection = nullptr;
	delete stats;
	delete traits;
	delete inventory;
	delete visualityProcessor;
	std::cout << "~Player()\n";
}

void Player::onDisconnect() {
	auto map = getLocationData()->getMap();
	if (map) {
		map->addEntityToRemovalQueue(this, RemovalReason::PLAYER_DISCONNECT);
	}
}

bool Player::sendDataToVisible(const ResponsePacket& packet) const {
	bool success = true;
	auto visibleSectors = getVisualityProcessor()->getVisibleSectors();
	std::for_each(visibleSectors.begin(), visibleSectors.end(), [&success, &packet](const std::pair<uint32_t, MapSector*>& pair) {
		auto sector = pair.second;
		success &= sector->sendDataToAllPlayer(packet);
	});
	return success;
}

bool Player::sendDataToVisibleExceptSelf(const ResponsePacket& packet) const {
	bool success = true;
	auto visibleSectors = getVisualityProcessor()->getVisibleSectors();
	std::for_each(visibleSectors.begin(), visibleSectors.end(), [&success, &packet](const std::pair<uint32_t, MapSector*>& pair) {
		auto sector = pair.second;
		success &= sector->sendDataToAllPlayerExcept(packet, nullptr);
	});
	return success;
}

bool Player::handleIdentification(const Packet* packet) {
	const IdentifyAccountRequestPacket* identPacket = dynamic_cast<const IdentifyAccountRequestPacket*>(packet);
	setAccountId(identPacket->getAccountId());

	if (!sendEncryption() || !loadEntirePlayer()) {
		return false;
	}

	bool success = getConnectionWrapper()->sendData(CharacterDataResponsePacket(this));
	success &= getConnectionWrapper()->sendData(InventoryResponsePacket(getInventory()));
	success &= getConnectionWrapper()->sendData(QuestDataResponsePacket());
	success &= getConnectionWrapper()->sendData(GamingPlanResponsePacket());

	return success;
}

bool Player::sendEncryption() {
	WorldEncryptionPacket encryptionPacket;
	encryptionPacket.setEncryptionKey(CryptTable::DEFAULT_CRYPTTABLE_START_VALUE);

	return getConnectionWrapper()->sendData(encryptionPacket);
}

bool Player::loadEntirePlayer() {
	WorldServer *server = WorldServer::getInstance();
	uint32_t charId = server->getLastLoggedCharacterFromAccount(accountId);
	getTraits()->setCharacterId(charId);
	bool success = server->loadCharacterDataForCharacter(this);
	success &= server->loadInventoryForCharacter(this);
	return success;
}

bool Player::handleAssignmentOfLocalId(const Packet* packet) {
	bool success = getLocationData()->getMap()->addEntityToInsertionQueue(this);

	AssignLocalPlayerIdResponsePacket response(this);

	CurrentWeightResponsePacket weightResponse;
	weightResponse.setLocalEntityId(getLocationData()->getLocalId());
	weightResponse.setWeightPercentage(0);

	StanceResponsePacket stanceResponse;
	stanceResponse.setLocalEntityId(getLocationData()->getLocalId());
	stanceResponse.setMovementSpeed(getStats()->getMovementSpeed());
	stanceResponse.setStanceType(0);

	success &= getConnectionWrapper()->sendData(response);
	success &= getConnectionWrapper()->sendData(weightResponse);
	success &= getConnectionWrapper()->sendData(stanceResponse);

	return success;
}

bool Player::handleChangedRespawnTown(const Packet* packet) {
	const ChangeRespawnTownRequestPacket* respawnTownPacket = dynamic_cast<const ChangeRespawnTownRequestPacket*>(packet);
	std::cout << "[HandleChangedRespawnTown-DEBUG]: " << respawnTownPacket->toPrintable().c_str() << "\n";
	return true;
}

bool Player::handleNewDestination(const Packet* packet) {
	const NewDestinationRequestPacket* destinationPacket = dynamic_cast<const NewDestinationRequestPacket*>(packet);

	Position destination = Position(destinationPacket->getDestinationX(), destinationPacket->getDestinationY());
	getLocationData()->getPositionCollection()->setDestinationPosition(destination);

	NewDestinationResponsePacket response;
	response.setEntityLocalId(getLocationData()->getLocalId());
	response.setLocalTargetId(destinationPacket->getTargetLocalId());
	response.setDestinationPosition(getLocationData()->getPositionCollection()->getDestinationPosition());

	return sendDataToVisible(response);
}

bool Player::spawnVisually(Entity* entity) {
	auto packet = entity->getVisualityProcessor()->createSpawnVisuallyPacket();
	return getConnectionWrapper()->sendData(*packet);
}

bool Player::despawnVisually(Entity* entity) {
	return getConnectionWrapper()->sendData(DespawnEntityVisuallyResponsePacket(entity));
}

bool Player::despawnVisually(uint16_t localId) {
	return getConnectionWrapper()->sendData(DespawnEntityVisuallyResponsePacket(localId));
}

bool Player::handleTelegateEntered(const Packet* packet) {
	const TelegateRequestPacket* telegatePacket = dynamic_cast<const TelegateRequestPacket*>(packet);
	const uint16_t telegateId = telegatePacket->getTelegateId();

	WorldServer *server = WorldServer::getInstance();
	return server->teleportPlayerFromTelegate(this, telegateId);
}

bool Player::handleCollision(const Packet* packet) {
	const CollisionRequestPacket* collisionPacket = dynamic_cast<const CollisionRequestPacket*>(packet);
	getLocationData()->getPositionCollection()->setCurrentPosition(collisionPacket->getCollisionPosition());
	getLocationData()->getPositionCollection()->setDestinationPosition(collisionPacket->getCollisionPosition());

	CollisionResponsePacket response;
	response.setEntityLocalId(getLocationData()->getLocalId());
	response.setCollisionPosition(getLocationData()->getPositionCollection()->getCurrentPosition());
	response.setZ(collisionPacket->getZ());

	return sendDataToVisible(response);
}

bool Player::handlePacket(const Packet* packet) {
	bool success = false;
	switch (packet->getCommandId()) {
		case PingRequestPacket::ID:
			success = true;
		break;
		case CollisionRequestPacket::ID:
			success = handleCollision(packet);
		break;
		case IdentifyAccountRequestPacket::ID:
			success = handleIdentification(packet);
		break;
		case AssignLocalPlayerIdRequestPacket::ID:
			success = handleAssignmentOfLocalId(packet);
		break;
		case ChangeRespawnTownRequestPacket::ID:
			success = handleChangedRespawnTown(packet);
		break;
		case NewDestinationRequestPacket::ID:
			success = handleNewDestination(packet);
		break;
		case TelegateRequestPacket::ID:
			success = handleTelegateEntered(packet);
		break;
		default:
			std::cout << "Unknown Packet: " << packet->toPrintable().c_str() << "\n";
	}
	return success;
}