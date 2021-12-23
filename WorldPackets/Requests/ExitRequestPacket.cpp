#include "ExitRequestPacket.h"

ExitRequestPacket::ExitRequestPacket(const Packet* packet) : Packet(ExitRequestPacket::ID, ExitRequestPacket::DEFAULT_LENGTH) {

}

ExitRequestPacket::~ExitRequestPacket() {

}