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