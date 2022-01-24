#ifndef __ROSE_UPDATE_QUEST_JOURNAL_QUEST_REWARD__
#define __ROSE_UPDATE_QUEST_JOURNAL_QUEST_REWARD__

#include "../../QSD.h"

enum class UpdateQuestJournalOperation : uint8_t {
	REMOVE_QUEST,
	START_QUEST,
	REPLACE_QUEST_AND_KEEP_ITEMS,
	REPLACE_QUEST_AND_LOSE_ITEMS,
	SELECT_QUEST
};

class UpdateQuestJournalQuestReward : public QuestReward {
private:
	uint32_t questId;
	UpdateQuestJournalOperation operation;
public:
	UpdateQuestJournalQuestReward(std::shared_ptr<char>& rawData);
	virtual ~UpdateQuestJournalQuestReward();
	virtual void performAction(QuestTriggerContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_UPDATE_QUEST_JOURNAL_QUEST_REWARD__