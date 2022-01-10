#ifndef __ROSE_QUEST_ACTION_FACTORY__
#define __ROSE_QUEST_ACTION_FACTORY__

#include "../QSD.h"

class QuestActionFactory {
public:
	using QuestActionFunctionPointer = std::unique_ptr<QuestAction>(*)(std::shared_ptr<char>&);
private:
	const static uint32_t CONDITION_OPERATIONCODE_START = 0x01000000;
	static QuestActionFunctionPointer creatorFunctions[];
	QuestActionFactory() {}
	virtual ~QuestActionFactory() {}
public:
	static std::unique_ptr<QuestAction> createActionFromRawData(std::shared_ptr<char>& rawData);
};

#endif 