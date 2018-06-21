#include "ChangeRespawnTownRequestPacket.h"

ChangeRespawnTownRequestPacket::ChangeRespawnTownRequestPacket(const Packet* p) : Packet(ID, p->getLength()) {
}

ChangeRespawnTownRequestPacket::~ChangeRespawnTownRequestPacket() {

}