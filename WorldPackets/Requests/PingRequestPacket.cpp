#include "PingRequestPacket.h"

PingRequestPacket::PingRequestPacket(const Packet* packet) : Packet(ID, packet->getLength()) {

}

PingRequestPacket::~PingRequestPacket() {

}