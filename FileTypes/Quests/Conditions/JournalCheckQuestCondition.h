#ifndef __ROSE_JOURNAL_CHECK_QUEST_CONDITION__
#define __ROSE_JOURNAL_CHECK_QUEST_CONDITION__

#include "../../QSD.h"

class JournalCheckQuestCondition : public QuestCondition {
private:
	uint32_t questId;
public:
	JournalCheckQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~JournalCheckQuestCondition();
	virtual bool isConditionFulfilled() const;
};

#endif //__ROSE_JOURNAL_CHECK_QUEST_CONDITION__