#include "CheckVariableQuestCondition.h"

CheckVariableQuestCondition::CheckVariableQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	amountOfChecks = reader.readUInt();
	checks = new QuestDataCheck[amountOfChecks + 1];
	for (uint32_t i = 0; i < amountOfChecks; i++) {
		checks[i] = QuestDataCheck(reader);
	}
}

CheckVariableQuestCondition::~CheckVariableQuestCondition() {

}

bool CheckVariableQuestCondition::isConditionFulfilled() const {
	return false;
}