#ifndef __ROSE_STAT_SUFFICIENT_CONDITION__
#define __ROSE_STAT_SUFFICIENT_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class StatSufficientCondition : public AICondition {
private:
	AIPAbilityType abilityType;
	uint32_t abilityAmountNecessary;
	CheckOperationType operation;
public:
	StatSufficientCondition(std::shared_ptr<char>& rawData);
	virtual ~StatSufficientCondition();

	virtual bool isFulfilled(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_STAT_SUFFICIENT_CONDITION__