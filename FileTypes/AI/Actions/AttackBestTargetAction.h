#ifndef __ROSE_ATTACK_BEST_TARGET_ACTION__
#define __ROSE_ATTACK_BEST_TARGET_ACTION__

#include "../../AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class AttackBestTargetAction : public AIAction {
private:
	float maximumAllowedDistance;
	AIPAbilityType abilityType;
	bool needsLessThanOtherValue;
public:
	AttackBestTargetAction(std::shared_ptr<char>& rawData);
	virtual ~AttackBestTargetAction();
	virtual void performAction(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_ATTACK_BEST_TARGET__