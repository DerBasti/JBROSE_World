#include "SpawnDropVisuallyResponsePacket.h"
#include "../../Entities/Drop.h"

SpawnDropVisuallyResponsePacket::SpawnDropVisuallyResponsePacket(Drop* drop) : SpawnEntityVisuallyResponsePacket(ID) {
	entityLocalId = drop->getLocationData()->getLocalId();
	currentPosition = drop->getLocationData()->getMapPosition()->getCurrentPosition();
	item = drop->getItem();
}

SpawnDropVisuallyResponsePacket::~SpawnDropVisuallyResponsePacket() {

}

void SpawnDropVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(currentPosition.getX());
	packet.addData(currentPosition.getY());
	packet.addData(ItemVisuality::toPacketHeader(item));
	packet.addData(item.getAmount());
	packet.addData(entityLocalId);
	packet.addData(ItemVisuality::toPacketBody(item));
}

std::string SpawnDropVisuallyResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[SpawnDropVisuallyResponsePacket] Spawning drop at %.2f, %.2f", currentPosition.getX(), currentPosition.getY());
	return std::string(buf);
}