#ifndef __ROSE_QUEST_ITEM_AMOUNT_SUFFICIENT_QUEST_CONDITION__
#define __ROSE_QUEST_ITEM_AMOUNT_SUFFICIENT_QUEST_CONDITION__

#include "../../QSD.h"


class QuestItemAmountSufficientQuestCondition : public QuestCondition {
private:
	uint32_t amountOfItems;
	RequestedQuestItem* requestedItems;
public:
	QuestItemAmountSufficientQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~QuestItemAmountSufficientQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_QUEST_ITEM_AMOUNT_SUFFICIENT_QUEST_CONDITION__