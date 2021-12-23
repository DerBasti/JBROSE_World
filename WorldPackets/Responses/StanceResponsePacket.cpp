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

std::string StanceResponsePacket::toPrintable() const {
	char buf[0x100] = { 0x00 };
	sprintf_s(buf, "[StanceResponsePacket]\n\t* Entity local id: %i\n\t* StanceID: %i\n\t* Resulting Movementspeed: %i", localId, stanceType, movementSpeed);
	return std::string(buf);
}