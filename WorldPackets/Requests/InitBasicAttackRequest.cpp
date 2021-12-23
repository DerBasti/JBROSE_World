#include "InitBasicAttackRequestPacket.h"

InitBasicAttackRequestPacket::InitBasicAttackRequestPacket(const Packet* p) : Packet(ID, p->getLength()) {
	LoadedDataReader interpreter(p->getRawData());
	targetLocalId = interpreter.readUShort();
}

InitBasicAttackRequestPacket::~InitBasicAttackRequestPacket() {

}

std::string InitBasicAttackRequestPacket::toPrintable() const {
	char buf[0xA0] = { 0x00 };
	sprintf_s(buf, "[InitBasicAttackRequestPacket]\n\t* Target's local id: %i", targetLocalId);
	return std::string(buf);
}