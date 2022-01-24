#include "BasicAttackResponsePacket.h"
#include "../../BasicTypes/Entity.h"

BasicAttackResponsePacket::BasicAttackResponsePacket(DamageHit hit) : damageHit(hit), ResponsePacket(ID, DEFAULT_PACKET_LENGTH) {
	this->damageHit = std::move(hit);
}

BasicAttackResponsePacket::~BasicAttackResponsePacket() {

}

void BasicAttackResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(damageHit.getAttackerLocalId());
	packet.addData(damageHit.getTargetLocalId());
	packet.addData(damageHit.getDamageAmount() & 0x7FF | damageHit.getHitFlag());
}

std::string BasicAttackResponsePacket::toPrintable() const {
	char buf[0x300] = { 0x00 };
	sprintf_s(buf, "[BasicAttackResponsePacket]\n\t* Attacker: #%i\n\t* Damagetaker: #%i\n\t* Damage amount: %i", damageHit.getAttackerLocalId(),
		damageHit.getTargetLocalId(), damageHit.getDamageAmount());
	return std::string(buf);
}