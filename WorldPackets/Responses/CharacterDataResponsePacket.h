#ifndef __WORLD_CHARACTERDATA_RESPONSE_PACKET__
#define __WORLD_CHARACTERDATA_RESPONSE_PACKET__

#pragma once

#include "..\..\..\JBROSE_Common\Packet.h"

class CharacterDataResponsePacket : public ResponsePacket {
private:
	uint8_t sex;
	uint16_t mapId;
	float xCoordinate;
	float yCoordinate;
	uint16_t saveSpotId;
	uint32_t faceStyle;
	uint32_t hairStyle;

	uint32_t itemsVisuality[8];
	uint8_t birthstone;
	uint16_t birthPlace;
	uint16_t jobId;
	uint8_t unionId;
	uint8_t unionRank;
	uint8_t unionFame;

	uint16_t skilledStrength;
	uint16_t skilledDexterity;
	uint16_t skilledIntelligence;
	uint16_t skilledConcentration;
	uint16_t skilledCharm;
	uint16_t skilledSensibility;

	uint16_t currentHp;
	uint16_t currentMp;

	uint32_t experiencePoints;
	uint16_t level;
	uint16_t statPoints;
	uint16_t skillPoints;
	uint8_t headSize;
	uint8_t bodySize;
	uint32_t unknown1;
	uint32_t unknown2;

	uint16_t unionPoints[10];
	uint32_t unknown3;
	uint32_t unknown4;
	uint8_t unknown5;

	uint16_t stamina;

	uint8_t unknown6[0x146];
	uint16_t learnedSkills[120];
	uint16_t quickbar[48];
	uint32_t characterId;
	std::string characterName;
protected:
	void appendContentToSendable(SendablePacket& packet) const;

	void setBasicInformation(const std::shared_ptr<class PlayerTraits>& traits);
	void setStats(const std::shared_ptr<class PlayerStats>& stats);
	void setUnionData();
	void setSkillData();
public:
	const static uint16_t ID = 0x715;
	CharacterDataResponsePacket(const class Player* player);
	virtual ~CharacterDataResponsePacket();

	std::string toPrintable() const;
};

#endif //__WORLD_CHARACTERDATA_RESPONSE_PACKET__