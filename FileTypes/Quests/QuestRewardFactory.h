#ifndef __ROSE_QUEST_REWARD_FACTORY__
#define __ROSE_QUEST_REWARD_FACTORY__

#include "../QSD.h"

class QuestRewardFactory {
public:
	using QuestRewardFunctionPointer = std::unique_ptr<QuestReward>(*)(std::shared_ptr<char>&);
private:
	const static uint32_t CONDITION_OPERATIONCODE_START = 0x01000000;
	static QuestRewardFunctionPointer creatorFunctions[];
	QuestRewardFactory() {}
	virtual ~QuestRewardFactory() {}
public:
	static std::unique_ptr<QuestReward> createActionFromRawData(std::shared_ptr<char>& rawData);
};

#endif 