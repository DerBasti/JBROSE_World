#include "CharacterDataResponsePacket.h"
#include "..\..\WorldClient.h"

CharacterDataResponsePacket::CharacterDataResponsePacket(const Player* player) : ResponsePacket(ID) {
	this->mapId = 22;
	auto current = player->getLocationData()->getPositionCollection()->getCurrentPosition();
	this->xCoordinate = current.getX();
	this->yCoordinate = current.getY();
	setBasicInformation(player->getTraits());
	setStats(player->getStats());
	setUnionData();
	setSkillData();
	unknown1 = unknown2 = unknown3 = unknown4 = 0;
	unknown5 = 0;

	auto inventory = player->getInventory();

	const uint8_t visibleSlots[] = {
		ItemTypeList::HEADGEAR.getInventorySlot(),
		ItemTypeList::ARMOR.getInventorySlot(),
		ItemTypeList::GLOVES.getInventorySlot(),
		ItemTypeList::SHOES.getInventorySlot(),
		ItemTypeList::FACE.getInventorySlot(),
		ItemTypeList::BACK.getInventorySlot(),
		ItemTypeList::WEAPON.getInventorySlot(),
		ItemTypeList::SHIELD.getInventorySlot()
	};

	for (uint8_t i = 0; i < 8; i++) {
		itemsVisuality[i] = ItemVisuality::toVisualityBytes(inventory->getItem(visibleSlots[i]));
	}

	for (uint16_t i = 0; i < 0x146; i++) {
		unknown6[i] = 0;
	}
}

CharacterDataResponsePacket::~CharacterDataResponsePacket() {

}

void CharacterDataResponsePacket::setBasicInformation(PlayerTraits* traits) {
	this->characterId = traits->getCharacterId();
	this->characterName = traits->getName();
	this->hairStyle = traits->getHairStyle();
	this->faceStyle = traits->getFaceStyle();
	this->sex = traits->getSex();
	this->bodySize = 0x64;
	this->headSize = 0x64;
	this->saveSpotId = traits->getSavedSpotId();
	this->birthPlace = 0;
	this->birthstone = 0;
}

void CharacterDataResponsePacket::setStats(PlayerStats* stats) {
	this->currentHp = stats->getCurrentHp();
	this->currentMp = stats->getCurrentMp();
	this->experiencePoints = stats->getExperiencePoints();
	this->level = stats->getLevel();
	this->jobId = stats->getJobId();
	this->skilledCharm = 10;
	this->skilledConcentration = 15;
	this->skilledDexterity = 15;
	this->skilledIntelligence = 15;
	this->skilledSensibility = 10;
	this->skilledStrength = 15;
	this->skillPoints = stats->getAvailableSkillPoints();
	this->stamina = stats->getStamina();
	this->statPoints = stats->getAvailableStatPoints();
}
void CharacterDataResponsePacket::setUnionData() {
	this->unionFame = 0;
	this->unionId = 0;
	this->unionRank = 0;
	for (uint8_t i = 0; i < 10; i++) {
		this->unionPoints[i] = 0;
	}
}
void CharacterDataResponsePacket::setSkillData() {
	for (uint16_t i = 0; i < 120; i++) {
		learnedSkills[i] = 0x00;
	}
	for (uint8_t i = 0; i < 32; i++) {
		quickbar[i] = 0x00;
	}
}

void CharacterDataResponsePacket::appendContentToSendable(SendablePacket& packet) const {
	packet.addData(sex);
	packet.addData(mapId);
	packet.addData(xCoordinate);
	packet.addData(yCoordinate);
	packet.addData(saveSpotId);
	packet.addData(faceStyle);
	packet.addData(hairStyle);

	for (uint8_t i = 0; i < 8; i++) {
		packet.addData(itemsVisuality[i]);
	}
	packet.addData(birthstone);
	packet.addData(birthPlace);
	packet.addData(jobId);
	packet.addData(unionId);
	packet.addData(unionRank);
	packet.addData(unionFame);

	packet.addData(skilledStrength);
	packet.addData(skilledDexterity);
	packet.addData(skilledIntelligence);
	packet.addData(skilledConcentration);
	packet.addData(skilledCharm);
	packet.addData(skilledSensibility);

	packet.addData(currentHp);
	packet.addData(currentMp);

	packet.addData(experiencePoints);
	packet.addData(level);
	packet.addData(statPoints);
	packet.addData(skillPoints);
	packet.addData(headSize);
	packet.addData(bodySize);
	packet.addData(unknown1);
	packet.addData(unknown2);

	for (uint8_t i = 0; i < 10; i++) {
		packet.addData(unionPoints[i]);
	}
	packet.addData(unknown3);
	packet.addData(unknown4);
	packet.addData(unknown5);

	packet.addData(stamina);

	for (uint16_t i = 0; i < 0x146; i++) {
		packet.addData(unknown6[i]);
	}
	for (uint8_t i = 0; i < 120; i++) {
		packet.addData(learnedSkills[i]);

	}
	for (uint8_t i = 0; i < 32; i++) {
		packet.addData(quickbar[i]);
	}

	packet.addData(characterId);
	packet.addString(characterName);	
}

std::string CharacterDataResponsePacket::toPrintable() const {
	char buf[0x200] = { 0x00 };
	sprintf_s(buf, "[CharacterDataResponsePacket]\n\t* Name: %s\n\t* Id: %i\n", characterName.c_str(), characterId);
	return std::string(buf);
}