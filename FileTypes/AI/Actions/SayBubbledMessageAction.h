#ifndef __ROSE_SAY_BUBBLED_MESSAGE_ACTION__
#define __ROSE_SAY_BUBBLED_MESSAGE_ACTION__

#include "../../AIP.h"

class SayBubbledMessageAction : public AIAction {
private:
	uint16_t messageTableId;
public:
	SayBubbledMessageAction(std::shared_ptr<char>& rawData);
	virtual ~SayBubbledMessageAction();
	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_SAY_BUBBLED_MESSAGE_ACTION__