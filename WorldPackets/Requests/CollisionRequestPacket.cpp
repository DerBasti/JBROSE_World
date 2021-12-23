#include "CollisionRequestPacket.h"

CollisionRequestPacket::CollisionRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), DEFAULT_LENGTH) {
	LoadedDataReader interpreter(packet->getRawData());
	float x = interpreter.readFloat();
	float y = interpreter.readFloat();

	collisionPosition = Position(x, y);
	z = interpreter.readUShort();
}

CollisionRequestPacket::~CollisionRequestPacket() {

}

std::string CollisionRequestPacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[CollisionRequestPacket]\n\t* Collision at: %.2f, %.2f", collisionPosition.getX(), collisionPosition.getY());
	return std::string(buf);
}