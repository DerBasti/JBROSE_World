#include "SpawnMonsterVisuallyResponsePacket.h"
#include "..\..\Entities\Monster.h"

SpawnMonsterVisuallyResponsePacket::SpawnMonsterVisuallyResponsePacket(Monster* monster) : SpawnEntityVisuallyResponsePacket(ID, monster) {
	typeId = monster->getDefaultStatValues()->getId();
	unknown = 0x00;
}

SpawnMonsterVisuallyResponsePacket::~SpawnMonsterVisuallyResponsePacket() {

}

void SpawnMonsterVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	SpawnEntityVisuallyResponsePacket::appendContentToSendable(packet);
	packet.addData(typeId);
	packet.addData(unknown);
}