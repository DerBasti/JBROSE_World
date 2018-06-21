#include "CollisionRequestPacket.h"


CollisionRequestPacket::CollisionRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), DEFAULT_LENGTH) {
	const float *floatPacketData = reinterpret_cast<const float*>(packet->getRawData());
	float x = *floatPacketData;
	floatPacketData++;

	float y = *floatPacketData;
	floatPacketData++;

	collisionPosition = Position(x, y);
	z = *reinterpret_cast<const uint16_t*>(floatPacketData);
}

CollisionRequestPacket::~CollisionRequestPacket() {

}