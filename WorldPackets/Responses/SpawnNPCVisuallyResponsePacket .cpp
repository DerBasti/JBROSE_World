#include "SpawnNPCVisuallyResponsePacket.h"
#include "..\..\Entities\NPC.h"

SpawnNPCVisuallyResponsePacket::SpawnNPCVisuallyResponsePacket(NPC* npc) : SpawnEntityVisuallyResponsePacket(ID, npc) {
	typeId = npc->getDefaultStatValues()->getId();
	conversationId = typeId - 900;
	direction = npc->getLocationData()->getPositionCollection()->getDirection();
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