#include "QuestRewardFactory.h"
#include "QuestRewards.h"

QuestRewardFactory::QuestRewardFunctionPointer QuestRewardFactory::creatorFunctions[] = {
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new UpdateQuestJournalQuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new ModifyItemsQuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new TeleportPlayerQuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new RunQuestTriggerQuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); },
	[](std::shared_ptr<char>& rawData) { return std::unique_ptr<QuestReward>(new QuestReward(rawData)); }
};

std::unique_ptr<QuestReward> QuestRewardFactory::createActionFromRawData(std::shared_ptr<char>& rawData) {
	uint32_t* rawDataAsInt = reinterpret_cast<uint32_t*>(rawData.get());
	uint32_t operationCode = rawDataAsInt[1] & 0xFFFF;

	auto result = creatorFunctions[operationCode](rawData);
	return result;
}