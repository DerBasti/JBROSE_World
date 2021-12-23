#ifndef __ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__
#define __ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class CompareAbilityValueOfEntityCondition : public AICondition {
private:
	AbilityType abilityType;
	OperationType operation;
public:
	CompareAbilityValueOfEntityCondition(std::shared_ptr<char>& datablock);
	virtual ~CompareAbilityValueOfEntityCondition() {}

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__