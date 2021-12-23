#include "DistributeStatPointsRequestPacket.h"


DistributeStatPointRequestPacket::DistributeStatPointRequestPacket(const Packet* packet) : Packet(ID, DEFAULT_LENGTH) {
	LoadedDataReader reader(packet->getRawData());
	statType = reader.readByte();
}

DistributeStatPointRequestPacket::~DistributeStatPointRequestPacket() {

}

std::string DistributeStatPointRequestPacket::toPrintable() const {
	char buf[0xA0] = { 0x00 };
	sprintf_s(buf, "[DistributeStatPointRequestPacket]\n\t* StatTypeId: %i", statType);
	return std::string(buf);
}