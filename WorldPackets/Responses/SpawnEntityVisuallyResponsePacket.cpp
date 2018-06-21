#include "SpawnEntityVisuallyResponsePacket.h"
#include "..\..\BasicTypes\Entity.h"

SpawnEntityVisuallyResponsePacket::SpawnEntityVisuallyResponsePacket(uint16_t id, Entity* entity) : ResponsePacket(id) {
	auto locationData = entity->getLocationData();
	entityLocalId = locationData->getLocalId();
	currentPosition = locationData->getPositionCollection()->getCurrentPosition();
	destinationPosition = locationData->getPositionCollection()->getDestinationPosition();

	if (currentPosition != destinationPosition) {
		currentAction = VisualityActionBits::MOVING;
	}
	else {
		currentAction = VisualityActionBits::IDLE;
	}

	targetLocalId = 0;
	currentStance = 0;
	currentHp = 50;
	teamId = 1;
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