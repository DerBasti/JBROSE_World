#ifndef __ROSE_SELECT_GLOBAL_NPC_QUEST_CONDITION__
#define __ROSE_SELECT_GLOBAL_NPC_QUEST_CONDITION__

#include "../../QSD.h"

class SelectGlobalNpcQuestCondition : public QuestCondition {
private:
	uint16_t npcId;
public:
	SelectGlobalNpcQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~SelectGlobalNpcQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_SELECT_GLOBAL_NPC_QUEST_CONDITION__