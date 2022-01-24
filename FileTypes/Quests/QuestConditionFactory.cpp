#include "QuestConditionFactory.h"
#include "QuestConditions.h"

QuestConditionFactory::QuestConditionFunctionPointer QuestConditionFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new SelectQuestFromJournalCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckVariableQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckVariableQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new CheckEntityAbilityQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestItemAmountSufficientQuestCondition(rawData)); }, //Check Items of selected quest
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); }, //Check Party
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new ValidateCurrentPositionQuestCondition(rawData)); }, //Check position
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); }, //Check time (between start and end)
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); }, //Check time (via operation)
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestCondition(rawData)); }, //Check Skill (between Skill1 and Skill2)
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new RandomPercentageQuestCondition(rawData)); }, //Check percentage (between rnd1 and rnd2)
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new AIVariableCheckQuestCondition(rawData)); }, //
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new EventPositionBoundryQuestCondition(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new SelectGlobalNpcQuestCondition(rawData)); }, //Select NPC
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestCondition>(new QuestFlagValidationQuestCondition(rawData)); },
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