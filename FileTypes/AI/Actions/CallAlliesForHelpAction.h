#ifndef __ROSE_CALL_ALLIES_FOR_HELP_ACTION__
#define __ROSE_CALL_ALLIES_FOR_HELP_ACTION__

#include "../../AIP.h"

class CallAlliesForHelpAction : public AIAction {
private:
	float callDistance;
	uint32_t amountOfAllies;
public:
	CallAlliesForHelpAction(std::shared_ptr<char>& rawData);
	virtual ~CallAlliesForHelpAction();
	virtual void performAction(AIContext& context);

	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_CALL_ALLIES_FOR_HELP_ACTION__