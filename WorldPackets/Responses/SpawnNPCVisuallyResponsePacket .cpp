#include "SpawnNPCVisuallyResponsePacket.h"
#include "..\..\Entities\NPC.h"

SpawnNPCVisuallyResponsePacket::SpawnNPCVisuallyResponsePacket(NPC* npc) : SpawnEntityVisuallyResponsePacket(ID, npc) {
	typeId = npc->getDefaultStatValues()->getId();
	conversationId = typeId - 900;
	direction = npc->getLocationData()->getMapPosition()->getDirection();
	unknown = 0x00;
}

SpawnNPCVisuallyResponsePacket::~SpawnNPCVisuallyResponsePacket() {

}

void SpawnNPCVisuallyResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	SpawnEntityVisuallyResponsePacket::appendContentToSendable(packet);
	packet.addData(getNPCTypeId());
	packet.addData(getConversationId());
	packet.addData(getDirection());
	packet.addData(unknown);
}

std::string SpawnNPCVisuallyResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[SpawnNPCVisuallyResponsePacket]\n\t* Type: %i\n\t* LocalId: %i\n\t* Current Position: (%.2f, %.2f)", typeId, getEntityLocalId(), getCurrentPosition().getX(), getCurrentPosition().getY());
	return std::string(buf);
}