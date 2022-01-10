#ifndef __ROSE_CHECK_DISTANCE_TO_TARGET_CONDITION__
#define __ROSE_CHECK_DISTANCE_TO_TARGET_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class CheckDistanceToTargetCondition : public AICondition {
private:
	uint32_t neededDistance;
	CheckOperationType operation;
public:
	CheckDistanceToTargetCondition(std::shared_ptr<char>& datablock);
	CheckDistanceToTargetCondition(uint32_t operationCode, uint32_t length, const char* contextData);
	virtual ~CheckDistanceToTargetCondition() {}

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_CHECK_DISTANCE_TO_TARGET_CONDITION__