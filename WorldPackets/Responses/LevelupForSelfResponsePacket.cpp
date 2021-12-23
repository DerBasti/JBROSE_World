#include "LevelupForSelfResponsePacket.h"
#include "../../WorldClient.h"

LevelupForSelfResponsePacket::LevelupForSelfResponsePacket(Player* player) : ResponsePacket(ID, DEFAULT_PACKET_LENGTH) {
	auto playerStats = player->getStats();
	this->localId = player->getLocationData()->getLocalId();
	this->experience = playerStats->getExperiencePoints();
	this->level = player->getStats()->getLevel();
	this->statPoints = playerStats->getAvailableStatPoints();
	this->skillPoints = playerStats->getAvailableSkillPoints();
}

LevelupForSelfResponsePacket::~LevelupForSelfResponsePacket() {

}

void LevelupForSelfResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(level);
	packet.addData(experience);
	packet.addData(statPoints);
	packet.addData(skillPoints);
}

std::string LevelupForSelfResponsePacket::toPrintable() const {
	char buf[0xC0] = { 0x00 };
	sprintf_s(buf, "[LevelupForSelfResponsePacket]\n\t* Levelup for local id: %i\n\t* New level: %i\n\t* Statpoints gained: %i\n\t* Skillpoints gained: %i", localId, level, statPoints, skillPoints);
	return std::string(buf);
}