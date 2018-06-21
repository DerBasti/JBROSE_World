#include "SpawnPlayerVisuallyResponsePacket.h"
#include "..\..\WorldClient.h"

SpawnPlayerVisuallyResponsePacket::SpawnPlayerVisuallyResponsePacket(Player* player) : SpawnEntityVisuallyResponsePacket(ID, player) {
	sex = player->getTraits()->getSex();
	movementSpeed = 425;
}

SpawnPlayerVisuallyResponsePacket::~SpawnPlayerVisuallyResponsePacket() {

}

void SpawnPlayerVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	SpawnEntityVisuallyResponsePacket::appendContentToSendable(packet);
	packet.addData(sex);
	packet.addData(movementSpeed);
}