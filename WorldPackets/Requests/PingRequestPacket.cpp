#include "PingRequestPacket.h"

PingRequestPacket::PingRequestPacket(const Packet* packet) : Packet(ID, packet->getLength()) {
}

PingRequestPacket::~PingRequestPacket() {

}

std::string PingRequestPacket::toPrintable() const {
	char buf[0x30] = { 0x00 };
	sprintf_s(buf, "[PingRequestPacket]\n\t* Pinging...");
	return std::string(buf);
}