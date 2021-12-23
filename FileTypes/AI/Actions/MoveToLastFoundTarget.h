#ifndef __ROSE_MOVE_TO_LAST_FOUND_TARGET__
#define __ROSE_MOVE_TO_LAST_FOUND_TARGET__

#include "../../AIP.h"

class MoveToLastFoundTargetAction : public AIAction {
private:
	uint8_t stanceId;
public:
	MoveToLastFoundTargetAction(std::shared_ptr<char>& rawAction);
	virtual ~MoveToLastFoundTargetAction();

	virtual void performAction(AIContext& context);
};

#endif //__ROSE_MOVE_TO_LAST_FOUND_TARGET__