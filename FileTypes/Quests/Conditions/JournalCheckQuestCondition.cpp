#include "JournalCheckQuestCondition.h"


JournalCheckQuestCondition::JournalCheckQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);
	questId = reader.readUInt();
}
JournalCheckQuestCondition::~JournalCheckQuestCondition() {

}

bool JournalCheckQuestCondition::isConditionFulfilled() const {
	return false;
}