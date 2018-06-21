#include "TelegateRequestPacket.h"


TelegateRequestPacket::TelegateRequestPacket(const Packet* packet) : Packet(ID, DEFAULT_LENGTH) {
	telegateId = *(reinterpret_cast<const uint16_t*>(packet->getRawData()));
}

TelegateRequestPacket::~TelegateRequestPacket() {

}