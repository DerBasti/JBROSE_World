#ifndef __ROSE_MAKE_FAMILY_ATTACK_SAME_TARGET_WITHIN_DISTANCE__
#define __ROSE_MAKE_FAMILY_ATTACK_SAME_TARGET_WITHIN_DISTANCE__

#include "../../AIP.h"

class MakeFamilyAttackSameTargetWithinDistanceAction : public AIAction {
private:
	float maxDistanceAllowed;
	ROSELogger logger;
public:
	MakeFamilyAttackSameTargetWithinDistanceAction(std::shared_ptr<char>& rawAction);
	virtual ~MakeFamilyAttackSameTargetWithinDistanceAction();

	virtual void performAction(AIContext& context);
};

#endif 