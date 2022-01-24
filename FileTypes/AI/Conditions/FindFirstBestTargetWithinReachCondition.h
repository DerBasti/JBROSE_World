#ifndef __ROSE_FIND_NEAREST_AND_BEST_TARGET_CONDITION__
#define __ROSE_FIND_NEAREST_AND_BEST_TARGET_CONDITION__

#include "../../AIP.h"

class FindFirstBestTargetWithinReachCondition : public AICondition {
private:
	float maximumAllowedDistance;
	int16_t allowedMinimumLevelDifference;
	int16_t allowedMaximumLevelDifference;
	bool checkForAllyFlag;
public:
	FindFirstBestTargetWithinReachCondition(std::shared_ptr<char>& rawData);
	virtual ~FindFirstBestTargetWithinReachCondition();
	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_FIND_NEAREST_AND_BEST_TARGET_CONDITION__