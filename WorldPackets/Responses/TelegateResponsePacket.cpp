#include "TelegateResponsePacket.h"
#include "../../WorldServer.h"

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

std::string TelegateResponsePacket::toPrintable() const {
	char buf[0x150] = { 0x00 };
	sprintf_s(buf, "[TelegateResponsePacket]\n\t* Teleporting Entity with local id: %i\n\t* Map: %i\n\t* Position: %.2f, %.2f", localEntityId, WorldServer::getInstance()->getMapById(mapId)->getName(), position.getX(), position.getY());
	return std::string(buf);
}