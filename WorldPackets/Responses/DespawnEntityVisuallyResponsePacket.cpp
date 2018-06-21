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