#include "StanceResponsePacket.h"

StanceResponsePacket::StanceResponsePacket() : ResponsePacket(ID, DEFAULT_LENGTH) {
	localId = 0;
	stanceType = 0;
	movementSpeed = 425;
}

StanceResponsePacket::~StanceResponsePacket() {

}

void StanceResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(stanceType);
	packet.addData(movementSpeed);
}