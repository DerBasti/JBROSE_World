#include "CurrentWeightRequestPacket.h"


CurrentWeightRequestPacket::CurrentWeightRequestPacket(const Packet* packet) {

}

CurrentWeightRequestPacket::~CurrentWeightRequestPacket() {

}

std::string CurrentWeightRequestPacket::toPrintable() const {
	char buf[0x60] = { 0x00 };
	sprintf_s(buf, "[CurrentWeightRequestPacket]\t* Request to recalculate weight");
	return std::string(buf);
}