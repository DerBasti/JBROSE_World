#ifndef __ROSE_SHOW_EMOTE_ACTION__
#define __ROSE_SHOW_EMOTE_ACTION__

#include "../../AIP.h"

class ShowEmoteAction : public AIAction {
private:
	uint16_t emoteId;
public:
	ShowEmoteAction(std::shared_ptr<char>& rawData);
	virtual ~ShowEmoteAction();
	virtual void performAction(AIContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_SHOW_EMOTE_ACTION__