#ifndef __ROSE_MOVE_TOWARDS_TARGET_ACTION__
#define __ROSE_MOVE_TOWARDS_TARGET_ACTION__

#include "../../AIP.h"
#include "../../../BasicTypes/Stance.h"

class MoveTowardsTargetAction : public AIAction {
private:
	float distanceToTarget;
	uint8_t stanceType;
public:
	MoveTowardsTargetAction(std::shared_ptr<char>& rawData);
	virtual ~MoveTowardsTargetAction();
	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;

};

#endif //__ROSE_MOVE_TOWARDS_TARGET_ACTION__