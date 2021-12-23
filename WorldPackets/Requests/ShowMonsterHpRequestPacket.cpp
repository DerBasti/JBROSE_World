#include "ShowMonsterHpRequestPacket.h"
#include "../../../JBROSE_Common/FileReader.h"


ShowMonsterHpRequestPacket::ShowMonsterHpRequestPacket(const Packet *packet) : Packet(ID, DEFAULT_LENGTH) {
	LoadedDataReader reader(packet->getRawData());
	monsterLocalId = reader.readUShort();
}

ShowMonsterHpRequestPacket::~ShowMonsterHpRequestPacket() {

}

std::string ShowMonsterHpRequestPacket::toPrintable() const {
	char buf[0x50] = { 0x00 };
	sprintf_s(buf, "[ShowMonsterHpRequestPacket]\n\t* Monster local id: %i", monsterLocalId);
	return std::string(buf);
}