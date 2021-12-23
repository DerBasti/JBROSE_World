#include "ShowMonsterHpResponsePacket.h"
#include "../../Entities/Monster.h"

ShowMonsterHpResponsePacket::ShowMonsterHpResponsePacket(Monster* monster) : ResponsePacket(ID) {
	monsterCurrentHp = monster->getStats()->getCurrentHp();
	monsterLocalId = monster->getLocationData()->getLocalId();
}
ShowMonsterHpResponsePacket::~ShowMonsterHpResponsePacket() {

}

void ShowMonsterHpResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(monsterLocalId);
	packet.addData(monsterCurrentHp);
}

std::string ShowMonsterHpResponsePacket::toPrintable() const {
	char buf[0x60] = { 0x00 };
	sprintf_s(buf, "[ShowMonsterHpResponsePacket]\n\t* Monster local id: %i\n\t* Current Hp: %i", monsterLocalId, monsterCurrentHp);
	return std::string(buf);
}