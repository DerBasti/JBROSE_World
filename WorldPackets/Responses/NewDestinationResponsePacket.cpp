#include "NewDestinationResponsePacket.h"

NewDestinationResponsePacket::NewDestinationResponsePacket() : NewDestinationResponsePacket(ID) {

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
}