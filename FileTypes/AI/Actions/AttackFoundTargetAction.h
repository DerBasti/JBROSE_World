#ifndef __ROSE_ATTACK_FOUND_TARGET_ACTION__
#define __ROSE_ATTACK_FOUND_TARGET_ACTION__

#include "../../AIP.h"

class AttackFoundTargetAction  : public AIAction {
public:
	AttackFoundTargetAction(std::shared_ptr<char>& rawData);
	virtual ~AttackFoundTargetAction();
	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_ATTACK_FOUND_TARGET_ACTION__