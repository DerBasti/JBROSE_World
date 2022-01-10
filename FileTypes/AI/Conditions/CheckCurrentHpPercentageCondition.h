#ifndef __ROSE_CHECK_CURRENT_HP_PERCENTAGE_CONDITION__
#define __ROSE_CHECK_CURRENT_HP_PERCENTAGE_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class CheckCurrentHpPercentageCondition : public AICondition {
private:
	uint32_t hpInPercent;
	CheckOperationType operation;
public:
	CheckCurrentHpPercentageCondition(std::shared_ptr<char>& datablock);
	virtual ~CheckCurrentHpPercentageCondition();

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_CHECK_CURRENT_HP_PERCENTAGE_CONDITION__