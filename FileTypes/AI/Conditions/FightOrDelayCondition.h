#ifndef __ROSE_FIGHT_OR_DELAY_CONDITION__
#define __ROSE_FIGHT_OR_DELAY_CONDITION__

#include "..\..\AIP.h"

class FightOrDelayCondition : public AICondition {
public:
	FightOrDelayCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {}
	virtual ~FightOrDelayCondition() {}

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_FIGHT_OR_DELAY_CONDITION__