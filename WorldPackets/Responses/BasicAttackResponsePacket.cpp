#include "BasicAttackResponsePacket.h"
#include "../../BasicTypes/Entity.h"

BasicAttackResponsePacket::BasicAttackResponsePacket(DamageHit hit) : damageHit(hit), ResponsePacket(ID, DEFAULT_PACKET_LENGTH) {
	this->damageHit = std::move(hit);
}

BasicAttackResponsePacket::~BasicAttackResponsePacket() {

}

void BasicAttackResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(damageHit.getAttacker()->getLocationData()->getLocalId());
	packet.addData(damageHit.getTarget()->getLocationData()->getLocalId());
	packet.addData(damageHit.getDamageAmount() & 0x7FF | damageHit.getHitFlag());
}

std::string BasicAttackResponsePacket::toPrintable() const {
	char buf[0x300] = { 0x00 };
	sprintf_s(buf, "[BasicAttackResponsePacket]\n\t* Attacker: %s (#%i)\n\t* Damagetaker: %s (#%i)\n\t* Damage amount: %i", damageHit.getAttacker()->getName(), damageHit.getAttacker()->getLocationData()->getLocalId(),
		damageHit.getTarget()->getName(), damageHit.getTarget()->getLocationData()->getLocalId(), damageHit.getDamageAmount());
	return std::string(buf);
}