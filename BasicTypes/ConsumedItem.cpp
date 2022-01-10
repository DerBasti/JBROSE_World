#include "RegenerationProcessor.h"
#include "../FileTypes/STB.h"

ConsumedItem::ConsumedItem() {
	influencedAbilityType = EntityAbilityType::UNKNOWN;
	alreadyConsumedValue = 0;
	maximumValue = 0;
	scriptExecutionType = ConsumableExecutionType::INVALID;
	valuePerSecond = 0;
	durationInMilliseconds = 0;
}

ConsumedItem::ConsumedItem(const ConsumedItem& otherItem) {
	influencedAbilityType = otherItem.influencedAbilityType;
	alreadyConsumedValue = otherItem.alreadyConsumedValue;
	maximumValue = otherItem.maximumValue;
	scriptExecutionType = otherItem.scriptExecutionType;
	valuePerSecond = otherItem.valuePerSecond;
	durationInMilliseconds = otherItem.durationInMilliseconds;
	timer.updateTimestamp();
}

ConsumedItem::ConsumedItem(const STBEntry* consumableEntry, const StatusSTBFile* statusSTBFile) {
	//entry->getColumnDataAsInt(ConsumeSTBFile::STAT_AMOUNT_REQUIRED_COLUMN);
	//entry->getColumnDataAsInt(ConsumeSTBFile::STAT_TYPE_REQUIRED_COLUMN);
	alreadyConsumedValue = 0;
	influencedAbilityType = OperationHandler::fromEntityAbilityTypeId(consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STAT_TYPE_TO_ADD_TO_COLUMN));
	maximumValue = consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STAT_AMOUNT_TO_ADD_TO_COLUMN);
	scriptExecutionType = static_cast<ConsumableExecutionType>(consumableEntry->getColumnDataAsInt(ConsumeSTBFile::SCRIPT_EXECUTION_TYPE_COLUMN));
	valuePerSecond = statusSTBFile->getIncreaseOfFirstValueOfEntry(consumableEntry->getColumnDataAsInt(ConsumeSTBFile::STATUS_STB_REFERENCE_COLUMN));
	if (valuePerSecond == 0) {
		valuePerSecond = static_cast<uint32_t>(-1);
	}
	durationInMilliseconds = (valuePerSecond == 0 ? 0 : maximumValue * 1000.0f / valuePerSecond);
}

ConsumedItem::~ConsumedItem() {

}

ConsumedItem& ConsumedItem::operator=(const ConsumedItem& otherItem) {
	influencedAbilityType = otherItem.influencedAbilityType;
	alreadyConsumedValue = otherItem.alreadyConsumedValue;
	maximumValue = otherItem.maximumValue;
	scriptExecutionType = otherItem.scriptExecutionType;
	valuePerSecond = otherItem.valuePerSecond;
	durationInMilliseconds = otherItem.durationInMilliseconds;
	timer.updateTimestamp();
	return (*this);
}

uint32_t ConsumedItem::updateUsageValue() {
	uint64_t passedTime = timer.getPassedTimeInMillis();
	if (getAlreadyConsumedValue() >= getMaximumValue()) {
		return 0;
	}
	uint32_t currentValue = std::ceil(passedTime * valuePerSecond / 1000.0f);
	uint32_t difference = currentValue - getAlreadyConsumedValue();
	if (difference > 0) {
		if (currentValue > getMaximumValue()) {
			currentValue = getMaximumValue();
			difference = getMaximumValue() - getAlreadyConsumedValue();
		}
		alreadyConsumedValue = currentValue;
	}
	return difference;
}


ConsumableItemList::ConsumableItemList(const class ConsumeSTBFile* consumablesSTBFile, const class StatusSTBFile* statusSTBFile) {
	for (uint32_t i = 0; i < consumablesSTBFile->getEntryAmount(); i++) {
		ConsumedItem item(consumablesSTBFile->getEntry(i), statusSTBFile);
		if (item.isValid()) {
			itemList.insert(std::make_pair(i, std::move(item)));
		}
	}
}

std::unique_ptr<ConsumedItem> ConsumableItemList::createConsumable(const uint16_t itemId) {
	auto iterator = itemList.find(itemId);
	if (iterator != itemList.end()) {
		return std::unique_ptr<ConsumedItem>(new ConsumedItem(iterator->second));
	}
	return std::unique_ptr<ConsumedItem>();
}