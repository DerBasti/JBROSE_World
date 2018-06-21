#include "TelegateResponsePacket.h"

TelegateResponsePacket::TelegateResponsePacket() : ResponsePacket(ID, DEFAULT_LENGTH) {
	z = 1;
}
TelegateResponsePacket::~TelegateResponsePacket() {

}

void TelegateResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localEntityId);
	packet.addData(mapId);
	packet.addData(position.getX());
	packet.addData(position.getY());
	packet.addData(z);
}