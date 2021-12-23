#include "NewDestinationResponsePacket.h"
#include "../../BasicTypes/Entity.h"

NewDestinationResponsePacket::NewDestinationResponsePacket(Entity* entity) : NewDestinationResponsePacket(entity->isPlayer() ? PLAYER_ID : NPC_ID) {
	entityId = entity->getLocationData()->getLocalId();
	localTargetId = entity->getCombat()->getTarget() != nullptr ? entity->getCombat()->getTarget()->getLocationData()->getLocalId() : 0x00;
	position = std::move(entity->getLocationData()->getMapPosition()->getDestinationPosition());
	unknown = static_cast<uint16_t>(entity->getPostionProcessor()->getDistanceToDestination());
	stance = entity->getStance()->getStanceId();
}

NewDestinationResponsePacket::NewDestinationResponsePacket(const uint16_t overrideId) : ResponsePacket(overrideId, DEFAULT_LENGTH) {
	entityId = 0;
	unknown = 0;
	z = 0;
}

NewDestinationResponsePacket::~NewDestinationResponsePacket() {

}

void NewDestinationResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(entityId);
	packet.addData(localTargetId);
	packet.addData(unknown);
	packet.addData(position.getX());
	packet.addData(position.getY());
	packet.addData(z);
	if (getCommandId() == NPC_ID) {
		packet.addData(stance);
	}
}

std::string NewDestinationResponsePacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[NewDestinationResponsePacket]\n\t* New destination (%.2f, %.2f) for local id: %i", position.getX(), position.getY(), entityId);
	return std::string(buf);
}