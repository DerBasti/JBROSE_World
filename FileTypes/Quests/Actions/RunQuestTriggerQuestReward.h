#ifndef __ROSE_RUN_QUEST_TRIGGER_QUEST_REWARD__
#define __ROSE_RUN_QUEST_TRIGGER_QUEST_REWARD__

#include "../../QSD.h"

class RunQuestTriggerQuestReward : public QuestReward {
private:
	std::shared_ptr<char> triggerName;
	uint32_t questHash;
public:
	RunQuestTriggerQuestReward(std::shared_ptr<char>& rawData);
	virtual ~RunQuestTriggerQuestReward();

	virtual void performAction(QuestTriggerContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_RUN_QUEST_TRIGGER_QUEST_REWARD__