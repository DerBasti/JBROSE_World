#ifndef __ROSE_AI_ACTION_FACTORY__
#define __ROSE_AI_ACTION_FACTORY__

#include "..\AIP.h"

class AIActionFactory {
public:
	using AIActionFunctionPointer = std::shared_ptr<AIAction>(*)(std::shared_ptr<char>&);
private:
	const static uint32_t CONDITION_OPERATIONCODE_START = 0x0B000001;
	static AIActionFunctionPointer creatorFunctions[];
	AIActionFactory() {}
	virtual ~AIActionFactory() {}
public:
	static std::shared_ptr<AIAction> createActionFromRawData(std::shared_ptr<char>& rawData);
};

#endif //__ROSE_AI_ACTION_FACTORY__