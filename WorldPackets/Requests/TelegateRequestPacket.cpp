#include "TelegateRequestPacket.h"
#include "../../../JBROSE_Common/FileReader.h"

TelegateRequestPacket::TelegateRequestPacket(const Packet* packet) : Packet(ID, DEFAULT_LENGTH) {
	LoadedDataReader reader(packet->getRawData());
	telegateId = reader.readUShort();
}

TelegateRequestPacket::~TelegateRequestPacket() {

}

std::string TelegateRequestPacket::toPrintable() const {
	char buf[0x50] = { 0x00 };
	sprintf_s(buf, "[TelegateRequestPacket]\n\t* TelegateId: %i", telegateId);
	return std::string(buf);
}