#include "CollisionResponsePacket.h"


CollisionResponsePacket::CollisionResponsePacket() : ResponsePacket(ID, DEFAULT_LENGTH) {

}
CollisionResponsePacket::~CollisionResponsePacket() {

}

void CollisionResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(collisionPosition.getX());
	packet.addData(collisionPosition.getY());
	packet.addData(z);
}

std::string CollisionResponsePacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[CollisionResponsePacket]\n\t* LocalId: %i\n\t* Collision at: %.2f, %.2f", localId, collisionPosition.getX(), collisionPosition.getY());
	return std::string(buf);
}