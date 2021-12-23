#include "PickDropRequestPacket.h"


PickupDropRequestPacket::PickupDropRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), packet->getLength()) {
	LoadedDataReader reader(packet->getRawData());
	localId = reader.readUShort();
}

PickupDropRequestPacket::~PickupDropRequestPacket() {

}

std::string PickupDropRequestPacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[PickupDropRequestPacket]\n\t* Picking up drop with local id: %i", localId);
	return std::string(buf);
}