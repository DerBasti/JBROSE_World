#include "AssignLocalPlayerIdResponsePacket.h"
#include "..\..\WorldClient.h"
#include "../../Map/Map.h"

AssignLocalPlayerIdResponsePacket::AssignLocalPlayerIdResponsePacket(const Player* player) : ResponsePacket(ID, DEFAULT_LENGTH) {
	localId = player->getLocationData()->getLocalId();
	currentHp = player->getStats()->getCurrentHp();
	currentMp = player->getStats()->getCurrentMp();
	experiencePoints = player->getStats()->getExperiencePoints();

	productRate = 0x64;
	updateTime = 0x0C1F4B79;
	worldRate = 0x64;
	townRate = 0x50;
	for (uint8_t i = 0; i < 11; i++) {
		itemRates[i] = 0x32;
	}

	pvpMapFlag = static_cast<uint32_t>(AssignLocalPlayerIdPVPFlag::NO_PVP);
	mapTime = static_cast<uint32_t>(player->getLocationData()->getMap()->getMapTime().getCurrentTimeInSeconds());
	teamId = player->getCombat()->getTeamId();
}

AssignLocalPlayerIdResponsePacket::~AssignLocalPlayerIdResponsePacket() {

}


void AssignLocalPlayerIdResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(currentHp);
	packet.addData(currentMp);
	packet.addData(experiencePoints);
	packet.addData<uint32_t>(0x00); //Exp?

	packet.addData(productRate);
	packet.addData(updateTime);
	packet.addData(worldRate);
	packet.addData(townRate);
	for (uint8_t i = 0; i < 11; i++) {
		packet.addData(itemRates[i]);
	}

	packet.addData(pvpMapFlag);
	packet.addData(mapTime);
	packet.addData(teamId);
}


std::string AssignLocalPlayerIdResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[AssignLocalPlayerIdResponsePacket]\n\t* New local id: %i\n\t* WorldTime: %i\n\t* TeamId: %i", localId, mapTime, teamId);
	return std::string(buf);
}