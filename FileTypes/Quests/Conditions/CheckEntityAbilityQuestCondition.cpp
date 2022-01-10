#include "CheckEntityAbilityQuestCondition.h"


CheckEntityAbilityQuestCondition::CheckEntityAbilityQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	amountOfChecks = reader.readUInt();
	checkTypes = new QuestAbilityTypeCheck[amountOfChecks + 1];
	for (uint32_t i = 0; i < amountOfChecks; i++) {
		checkTypes[i] = QuestAbilityTypeCheck(reader);
	}
}

CheckEntityAbilityQuestCondition::~CheckEntityAbilityQuestCondition() {
	if (checkTypes) {
		delete[] checkTypes;
		checkTypes = nullptr;
	}
}

bool CheckEntityAbilityQuestCondition::isConditionFulfilled() const {
	return false;
}