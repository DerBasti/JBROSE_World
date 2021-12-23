#include "ExitResponsePacket.h"


ExitResponsePacket::ExitResponsePacket() : ResponsePacket(ExitResponsePacket::ID) {
	timeUntilDisconnect = 1;
}

ExitResponsePacket::~ExitResponsePacket() {

}

void ExitResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(timeUntilDisconnect);
}

std::string ExitResponsePacket::toPrintable() const {
	char buf[0xA0] = { 0x00 };
	sprintf_s(buf, "[ExitResponsePacket]\n\t* Time until disconnect: %i", timeUntilDisconnect);
	return std::string(buf);
}