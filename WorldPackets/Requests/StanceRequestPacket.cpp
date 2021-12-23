#include "StanceRequestPacket.h"

StanceRequestPacket::StanceRequestPacket(const Packet *packet) : Packet(ID, packet->getLength()) {
	stanceId = *packet->getRawData();
}

StanceRequestPacket::~StanceRequestPacket() {

}

std::string StanceRequestPacket::toPrintable() const {
	char buf[0x70] = { 0x00 };
	sprintf_s(buf, "[StanceRequestPacket]\n\t* StanceID: %i", stanceId);
	return std::string(buf);
}