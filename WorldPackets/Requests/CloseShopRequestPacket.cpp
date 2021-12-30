#include "CloseShopRequestPacket.h"

CloseShopRequestPacket::CloseShopRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), packet->getLength()) {

}

CloseShopRequestPacket::~CloseShopRequestPacket() {

}