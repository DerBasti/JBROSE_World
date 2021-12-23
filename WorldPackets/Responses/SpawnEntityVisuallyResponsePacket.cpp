#include "SpawnEntityVisuallyResponsePacket.h"
#include "..\..\BasicTypes\Entity.h"

SpawnEntityVisuallyResponsePacket::SpawnEntityVisuallyResponsePacket(uint16_t packetId) : ResponsePacket(packetId) {

}

SpawnEntityVisuallyResponsePacket::SpawnEntityVisuallyResponsePacket(uint16_t id, Entity* entity) : SpawnEntityVisuallyResponsePacket(id) {
	auto locationData = entity->getLocationData();
	entityLocalId = locationData->getLocalId();
	currentPosition = locationData->getMapPosition()->getCurrentPosition();
	destinationPosition = locationData->getMapPosition()->getDestinationPosition();

	if (currentPosition != destinationPosition) {
		currentAction = VisualityAction::MOVING;
	}
	else {
		currentAction = VisualityAction::IDLE;
	}

	targetLocalId = entity->getCombat()->getTarget() != nullptr ? entity->getCombat()->getTarget()->getLocationData()->getLocalId() : 0x00;
	currentStance = entity->getStance()->getStanceId();
	currentHp = entity->getStats()->getCurrentHp();
	teamId = entity->getCombat()->getTeamId();
	buffBits = 0;
}

SpawnEntityVisuallyResponsePacket::~SpawnEntityVisuallyResponsePacket() {
	  
}

void SpawnEntityVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(entityLocalId);
	packet.addData(currentPosition.getX());
	packet.addData(currentPosition.getY());
	packet.addData(destinationPosition.getX());
	packet.addData(destinationPosition.getY());
	packet.addData(static_cast<uint16_t>(currentAction));
	packet.addData(targetLocalId);
	packet.addData(currentStance);
	packet.addData(currentHp);
	packet.addData(teamId);
	packet.addData(buffBits);
}

std::string SpawnEntityVisuallyResponsePacket::toPrintable() const {
	char buf[0x60] = { 0x00 };
	sprintf_s(buf, "[SpawnEntityVisuallyResponsePacket]\n\t* Entity local id: %i\n\t* Position: %.2f, %.2f", entityLocalId, currentPosition.getX(), currentPosition.getY());
	return std::string(buf);
}