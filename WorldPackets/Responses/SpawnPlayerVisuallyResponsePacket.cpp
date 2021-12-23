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

std::string SpawnPlayerVisuallyResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[SpawnPlayerVisuallyResponsePacket]\n\t* LocalId: %i\n* Current Position: (%.2f, %.2f)", getEntityLocalId(), getCurrentPosition().getX(), getCurrentPosition().getY());
	return std::string(buf);
}