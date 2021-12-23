#include "LevelupForOthersResponsePacket.h"

LevelupForOthersResponsePacket::LevelupForOthersResponsePacket(uint16_t localId) : ResponsePacket(ID, DEFAULT_PACKET_LENGTH) {
	this->localId = localId;
}

LevelupForOthersResponsePacket::~LevelupForOthersResponsePacket() {

}

void LevelupForOthersResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
}

std::string LevelupForOthersResponsePacket::toPrintable() const {
	char buf[0xC0] = { 0x00 };
	sprintf_s(buf, "[LevelupForOthersResponsePacket]\n\t* Levelup for local id: %i", localId);
	return std::string(buf);
}