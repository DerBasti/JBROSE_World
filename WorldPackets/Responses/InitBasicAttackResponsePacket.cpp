#include "InitBasicAttackResponsePacket.h"
#include "../../BasicTypes/Combat.h"
#include "../../BasicTypes/Entity.h"
	
InitBasicAttackResponsePacket::InitBasicAttackResponsePacket(Combat* combat) : ResponsePacket(InitBasicAttackResponsePacket::ID) {
	this->selfLocalId = combat->getEntitySelf()->getLocationData()->getLocalId();
	this->targetLocalId = combat->getTarget()->getLocationData()->getLocalId();
	this->targetPosition = combat->getTarget()->getLocationData()->getMapPosition()->getCurrentPosition();
	this->unknown = 0;
}

InitBasicAttackResponsePacket::~InitBasicAttackResponsePacket() {

}

void InitBasicAttackResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(selfLocalId);
	packet.addData(targetLocalId);
	packet.addData(unknown);
	packet.addData(targetPosition.getX());
	packet.addData(targetPosition.getY());
}

std::string InitBasicAttackResponsePacket::toPrintable() const {
	char buf[0xC0] = { 0x00 };
	sprintf_s(buf, "[InitBasicAttackRequestPacket]\n\t* Attackers's local id: %i\n\t* Target's local id: %i\n\t* Position: %.2f, %.2f", selfLocalId, targetLocalId, targetPosition.getX(), targetPosition.getY());
	return std::string(buf);
}