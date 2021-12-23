#include "CurrentWeightResponsePacket.h"

CurrentWeightResponsePacket::CurrentWeightResponsePacket() : ResponsePacket(ID, DEFAULT_LENGTH) {
	localPlayerId = 0;
	weightPercentage = 0;
}

CurrentWeightResponsePacket::~CurrentWeightResponsePacket() {

}

void CurrentWeightResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localPlayerId);
	packet.addData(weightPercentage);
}

std::string CurrentWeightResponsePacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[CurrentWeightResponsePacket]\n\t* LocalId: %i\n\t* Weight Percentage: %i", localPlayerId, weightPercentage);
	return std::string(buf);
}