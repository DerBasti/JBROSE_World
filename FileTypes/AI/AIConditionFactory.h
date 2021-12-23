#ifndef __ROSE_AI_CONDITION_FACTORY__
#define __ROSE_AI_CONDITION_FACTORY__

#include "..\AIP.h"

class AIConditionFactory {
public:
	using AIConditionFunctionPointer = std::shared_ptr<AICondition>(*)(std::shared_ptr<char>&);
private:
	const static uint32_t CONDITION_OPERATIONCODE_START = 0x04000001;
	static AIConditionFunctionPointer creatorFunctions[];
	AIConditionFactory() {}
	virtual ~AIConditionFactory() {}
public:
	static std::shared_ptr<AICondition> createConditionFromRawData(std::shared_ptr<char>& rawData);
};

#endif //__ROSE_AI_CONDITION_FACTORY__