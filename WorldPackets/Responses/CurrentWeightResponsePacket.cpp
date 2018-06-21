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