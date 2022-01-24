#ifndef __ROSE_MOVE_TO_RANDOM_POINT_FROM_SPAWN_ACTION__
#define __ROSE_MOVE_TO_RANDOM_POINT_FROM_SPAWN_ACTION__

#include "../../AIP.h"

class MoveToRandomPointFromSpawnPositionAction : public AIAction {
private:
	float maxTravelDistance;
	uint8_t travelStance;
public:
	MoveToRandomPointFromSpawnPositionAction(std::shared_ptr<char>& rawAction);
	virtual ~MoveToRandomPointFromSpawnPositionAction();

	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;

};
#endif //__ROSE_MOVE_TO_RANDOM_POINT_FROM_SPAWN_ACTION__