#ifndef __ROSE_HAS_ENOUGH_TARGETS_CONDITION__
#define __ROSE_HAS_ENOUGH_TARGETS_CONDITION__

#include "../../AIP.h"

class HasEnoughTargetsCondition : public AICondition {
private:
	float maxAllowedDistanceFromTarget;
	bool alliedFlag;
	int16_t lowerLevelBoundry;
	int16_t upperLevelBoundry;
	uint16_t maximumOfFoundEntitiesNecessary;
public:
	HasEnoughTargetsCondition(std::shared_ptr<char>& datablock);
	HasEnoughTargetsCondition(uint32_t operationCode, uint32_t length, const char* contextData);
	virtual ~HasEnoughTargetsCondition() {}

	bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_HAS_ENOUGH_TARGETS_CONDITION__