#ifndef __ROSE_RANDOM_PERCENTAGE_CONDITION__
#define __ROSE_RANDOM_PERCENTAGE_CONDITION__

#include "../../AIP.h"

class RandomPercentageCondition : public AICondition {
private:
	uint8_t percentage;
public: 
	RandomPercentageCondition(std::shared_ptr<char>& rawData);
	virtual ~RandomPercentageCondition();

	virtual bool isFulfilled(AIContext& context);
};

#endif //__ROSE_RANDOM_PERCENTAGE_CONDITION__