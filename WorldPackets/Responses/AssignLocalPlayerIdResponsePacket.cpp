#include "AssignLocalPlayerIdResponsePacket.h"
#include "..\..\WorldClient.h"

AssignLocalPlayerIdResponsePacket::AssignLocalPlayerIdResponsePacket(const Player* player) : ResponsePacket(ID, DEFAULT_LENGTH) {
	localId = player->getLocationData()->getLocalId();
	currentHp = player->getStats()->getCurrentHp();
	currentMp = player->getStats()->getCurrentMp();
	experiencePoints = player->getStats()->getExperiencePoints();

	unknown1 = 0x64;
	unknown2 = 0x0C1F4B79;
	unknown3 = 0x64;
	unknown4 = 0x3232cd50;
	unknown5 = 0x32323235;
	unknown6 = 0x35323232;

	pvpMapFlag = 0;
	mapTime = 0;
	teamId = 0;
}

AssignLocalPlayerIdResponsePacket::~AssignLocalPlayerIdResponsePacket() {
}


void AssignLocalPlayerIdResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(localId);
	packet.addData(currentHp);
	packet.addData(currentMp);
	packet.addData(experiencePoints);

	packet.addData(unknown1);
	packet.addData(unknown2);
	packet.addData(unknown3);
	packet.addData(unknown4);
	packet.addData(unknown5);
	packet.addData(unknown6);

	packet.addData(pvpMapFlag);
	packet.addData(mapTime);
	packet.addData(teamId);
}
