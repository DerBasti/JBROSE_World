#ifndef __ROSE_RUN_AWAY_FROM_TARGET_ACTION__
#define __ROSE_RUN_AWAY_FROM_TARGET_ACTION__

#include "../../AIP.h"

class RunAwayFromTargetAction : public AIAction {
private:
	float runDistance;
public:
	RunAwayFromTargetAction(std::shared_ptr<char>& rawData);
	virtual ~RunAwayFromTargetAction();

	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_RUN_AWAY_FROM_TARGET_ACTION__