#ifndef __ROSE_DISTANCE_FROM_SPAWN_CONDITION__
#define __ROSE_DISTANCE_FROM_SPAWN_CONDITION__

#include "..\..\AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class DistanceFromSpawnCondition : public AICondition {
private:
	float allowedMaximumDistance;
	OperationType operation;
public:
	DistanceFromSpawnCondition(std::shared_ptr<char>& datablock);
	virtual ~DistanceFromSpawnCondition();

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_DISTANCE_FROM_SPAWN_CONDITION__