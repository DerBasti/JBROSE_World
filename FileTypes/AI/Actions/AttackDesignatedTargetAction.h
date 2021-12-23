#ifndef __ROSE_ATTACK_DESIGNATED_TARGET_ACTION__
#define __ROSE_ATTACK_DESIGNATED_TARGET_ACTION__

#include "../../AIP.h"

class AttackDesignatedTargetAction : public AIAction {
public:
	AttackDesignatedTargetAction() : AIAction(0, 0, nullptr) {}
	virtual ~AttackDesignatedTargetAction() {}

	virtual void performAction(AIContext& context);
};

#endif //__ROSE_ATTACK_DESIGNATED_TARGET_ACTION__