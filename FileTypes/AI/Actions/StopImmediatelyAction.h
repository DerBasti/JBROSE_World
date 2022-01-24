#ifndef __ROSE_STOP_IMMEDIATELY_ACTION__
#define __ROSE_STOP_IMMEDIATELY_ACTION__

#include "../../AIP.h"

class StopImmediatelyAction : public AIAction {
public:
	StopImmediatelyAction(std::shared_ptr<char>& rawData);
	virtual ~StopImmediatelyAction();
	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_STOP_IMMEDIATELY_ACTION__