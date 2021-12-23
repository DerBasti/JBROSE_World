#include "UpdateExperienceResponsePacket.h"
#include "../../WorldClient.h"

UpdateExperienceResponsePacket::UpdateExperienceResponsePacket(Player* player) : ResponsePacket(ID, DEFAULT_PACKET_LENGTH) {
	experience = player->getStats()->getExperiencePoints();
	stamina = 5000;
	unknown = 0;
}

UpdateExperienceResponsePacket::~UpdateExperienceResponsePacket() {

}

void UpdateExperienceResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(experience);
	packet.addData(stamina);
	packet.addData(unknown);
}

std::string UpdateExperienceResponsePacket::toPrintable() const {
	char buf[0x70] = { 0x00 };
	sprintf_s(buf, "[UpdateExperienceResponsePacket]\n\t* Experience: %u", experience);
	return std::string(buf);
}