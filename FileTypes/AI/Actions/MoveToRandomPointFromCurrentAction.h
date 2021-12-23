#ifndef __ROSE_MOVE_TO_RANDOM_POINT_FROM_CURRENT_ACTION__
#define __ROSE_MOVE_TO_RANDOM_POINT_FROM_CURRENT_ACTION__

#include "../../AIP.h"

class MoveToRandomPointFromCurrentPositionAction : public AIAction {
private:
	float maxTravelDistance;
	uint8_t travelStance;
public:
	MoveToRandomPointFromCurrentPositionAction(std::shared_ptr<char>& rawAction);
	virtual ~MoveToRandomPointFromCurrentPositionAction();

	virtual void performAction(AIContext& context);

};
#endif //__ROSE_MOVE_TO_RANDOM_POINT_FROM_CURRENT_ACTION__