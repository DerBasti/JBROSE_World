#ifndef __ROSE_ATTACK_NEAREST_TARGET_ACTION__
#define __ROSE_ATTACK_NEAREST_TARGET_ACTION__

#include "../../AIP.h"

class AttackNearestTargetAction : public AIAction {
public:
	AttackNearestTargetAction(std::shared_ptr<char>& rawData);
	virtual ~AttackNearestTargetAction();
	virtual void performAction(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif 