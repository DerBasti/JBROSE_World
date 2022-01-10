#ifndef __ROSE_QUEST_CONDITION_FACTORY__
#define __ROSE_QUEST_CONDITION_FACTORY__

#include "../QSD.h"

class QuestConditionFactory {
public:
	using QuestConditionFunctionPointer = std::unique_ptr<QuestCondition>(*)(std::shared_ptr<char>&);
private:
	static QuestConditionFunctionPointer creatorFunctions[];
	QuestConditionFactory() {}
	virtual ~QuestConditionFactory() {}
public:
	static std::unique_ptr<QuestCondition> createConditionFromRawData(std::shared_ptr<char>& rawData);
};

#endif //__ROSE_QUEST_CONDITION_FACTORY__