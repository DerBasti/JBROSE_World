#ifndef __ROSE_JOURNAL_CHECK_QUEST_CONDITION__
#define __ROSE_JOURNAL_CHECK_QUEST_CONDITION__

#include "../../QSD.h"

class SelectQuestFromJournalCondition : public QuestCondition {
private:
	uint32_t questId;
public:
	SelectQuestFromJournalCondition(std::shared_ptr<char>& rawData);
	virtual ~SelectQuestFromJournalCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_JOURNAL_CHECK_QUEST_CONDITION__