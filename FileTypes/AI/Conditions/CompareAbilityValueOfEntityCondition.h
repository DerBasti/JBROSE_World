#ifndef __ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__
#define __ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class CompareAbilityValueOfEntityCondition : public AICondition {
private:
	AIPAbilityType abilityType;
	CheckOperationType operation;
public:
	CompareAbilityValueOfEntityCondition(std::shared_ptr<char>& datablock);
	virtual ~CompareAbilityValueOfEntityCondition() {}

	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_COMPARE_ABILITY_VALUE_OF_ENTITY_CONDITION__