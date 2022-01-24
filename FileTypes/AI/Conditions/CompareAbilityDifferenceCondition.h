#ifndef __ROSE_COMPARE_ABILITY_DIFFERENCE_CONDITION__
#define __ROSE_COMPARE_ABILITY_DIFFERENCE_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class CompareAbilityDifferenceCondition : public AICondition {
private:
	AIPAbilityType abilityType;
	int32_t allowedDifference;
	CheckOperationType operation;
public: 
	CompareAbilityDifferenceCondition(std::shared_ptr<char>& rawData);
	virtual ~CompareAbilityDifferenceCondition();

	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_COMPARE_ABILITY_DIFFERENCE_CONDITION__