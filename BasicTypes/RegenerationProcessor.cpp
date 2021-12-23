#include "RegenerationProcessor.h"
#include "../WorldClient.h"
#include "../FileTypes/STB.h"

RegenerationProcessor::RegenerationProcessor(Player* player) {
	this->player = player;
}

RegenerationProcessor::~RegenerationProcessor() {
	player = nullptr;
}

void RegenerationProcessor::checkRegeneration() {
	if (!player->isIngame()) {
		naturalHealTimer.updateTimestamp();
		return;
	}
	checkRegenerationFromItems();
	checkRegenerationFromNaturalHealing();
}

void RegenerationProcessor::checkRegenerationFromItems() {

}

void RegenerationProcessor::checkRegenerationFromNaturalHealing() {
	uint64_t timePassed = naturalHealTimer.getPassedTimeInMillis();
	if (timePassed < RegenerationProcessor::TIME_TILL_NATURAL_HEAL_IN_MILLIS) {
		return;
	}
	float healingAmountForHp = std::ceil(player->getStats()->getMaxHp() * 2.0f / 100.0f);
	float healingAmountForMp = std::ceil(player->getStats()->getMaxMp() * 2.0f / 100.0f);
	if (player->getStance()->isSitting()) {
		healingAmountForHp *= 4;
		healingAmountForMp *= 4;
	}
	uint32_t actualHealingAmountForHp = static_cast<uint32_t>(healingAmountForHp);
	uint32_t actualHealingAmountForMp = static_cast<uint32_t>(healingAmountForMp);
	logger.logDebug("Healing naturally for ", actualHealingAmountForHp, "Hp and ", actualHealingAmountForMp, "Mp");
	player->getStats()->addToCurrentHp(actualHealingAmountForHp);
	player->getStats()->addToCurrentMp(actualHealingAmountForMp);

	naturalHealTimer.updateTimestamp();
}

ConsumedItem::ConsumedItem() {
	abilityType = 0;
	alreadyConsumedValue = 0;
	maximumValue = 0;
	scriptExecutionType = 0;
	valuePerSecond = 0;
}

ConsumedItem::ConsumedItem(STBEntry* consumableEntry, StatusSTBFile* statusEntry) {
	//entry->getColumnDataAsInt(ConsumeSTBFile::STAT_AMOUNT_REQUIRED_COLUMN);
	//entry->getColumnDataAsInt(ConsumeSTBFile::STAT_TYPE_REQUIRED_COLUMN);
	alreadyConsumedValue = 0;
	abilityType = consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STAT_TYPE_TO_ADD_TO_COLUMN);
	maximumValue = consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STAT_AMOUNT_TO_ADD_TO_COLUMN);
	scriptExecutionType = consumableEntry->getColumnDataAsInt(ConsumeSTBFile::SCRIPT_EXECUTION_TYPE_COLUMN);
	valuePerSecond = statusEntry->getIncreaseOfFirstValueOfEntry(consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STATUS_STB_REFERENCE_COLUMN));
}

ConsumedItem::~ConsumedItem() {

}

void ConsumableItemList::loadListFromStb(const ConsumeSTBFile* consumablesSTBFile) {

}

ConsumedItem ConsumableItemList::createConsumable(const uint16_t itemId) {
	auto iterator = itemList.find(itemId);
	if (iterator != itemList.end()) {
		return iterator->second;
	}
	return ConsumedItem();
}

std::map<uint16_t, ConsumedItem> ConsumableItemList::itemList;