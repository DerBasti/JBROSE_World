#ifndef __ROSE_HAS_TARGET_CHANGED_CONDITION__
#define __ROSE_HAS_TARGET_CHANGED_CONDITION__

#include "../../AIP.h"

class HasTargetChangedCondition : public AICondition {
public:
	HasTargetChangedCondition(std::shared_ptr<char>& rawData);
	virtual ~HasTargetChangedCondition();
	virtual bool isFulfilled(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_HAS_TARGET_CHANGED_CONDITION__