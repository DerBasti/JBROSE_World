#include "QuestConditionFactory.h"
#include "QuestConditions.h"

QuestConditionFactory::QuestConditionFunctionPointer QuestConditionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new JournalCheckQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckVariableQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckVariableQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckEntityAbilityQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); }
};

std::unique_ptr<QuestCondition> QuestConditionFactory::createConditionFromRawData(std::shared_ptr<char>& rawData) {
	uint32_t* rawDataAsInt = reinterpret_cast<uint32_t*>(rawData.get());
	uint32_t operationCode = rawDataAsInt[1];

	auto result = creatorFunctions[operationCode](rawData);
	return result;
}