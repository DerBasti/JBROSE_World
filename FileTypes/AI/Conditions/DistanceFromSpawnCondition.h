#ifndef __ROSE_DISTANCE_FROM_SPAWN_CONDITION__
#define __ROSE_DISTANCE_FROM_SPAWN_CONDITION__

#include "..\..\AIP.h"
#include "../../../BasicTypes/OperationHandler.h"

class DistanceFromSpawnCondition : public AICondition {
private:
	float allowedMaximumDistance;
	CheckOperationType operation;
public:
	DistanceFromSpawnCondition(std::shared_ptr<char>& datablock);
	virtual ~DistanceFromSpawnCondition();

	virtual bool isFulfilled(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_DISTANCE_FROM_SPAWN_CONDITION__