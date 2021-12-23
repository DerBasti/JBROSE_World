#include "DespawnEntityVisuallyResponsePacket.h"
#include "..\..\BasicTypes\Entity.h"

DespawnEntityVisuallyResponsePacket::DespawnEntityVisuallyResponsePacket(Entity* entity) : DespawnEntityVisuallyResponsePacket(entity->getLocationData()->getLocalId()) {

}

DespawnEntityVisuallyResponsePacket::DespawnEntityVisuallyResponsePacket(uint16_t localId) : ResponsePacket(ID, 10) {
	this->localId = localId;
}

DespawnEntityVisuallyResponsePacket::~DespawnEntityVisuallyResponsePacket() {

}
void DespawnEntityVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
}

std::string DespawnEntityVisuallyResponsePacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[DespawnEntityVisuallyResponsePacket]\n\t* LocalId: %i", localId);
	return std::string(buf);
}