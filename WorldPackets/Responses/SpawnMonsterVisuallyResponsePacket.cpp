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

std::string SpawnMonsterVisuallyResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[SpawnMonsterVisuallyResponsePacket]\n\t* Type: %i\n\t* LocalId: %i\n\t* Current Position: (%.2f, %.2f)", typeId, getEntityLocalId(), getCurrentPosition().getX(), getCurrentPosition().getY());
	return std::string(buf);
}