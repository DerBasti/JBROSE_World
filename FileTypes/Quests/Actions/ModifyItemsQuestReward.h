#ifndef __ROSE_MODIFY_ITEMS_QUEST_REWARDS__
#define __ROSE_MODIFY_ITEMS_QUEST_REWARDS__

#include "../../QSD.h"
#include "../../../BasicTypes/OperationHandler.h"

class ModifyItemsQuestReward : public QuestReward {
private:
	Item item;
	ResultOperationType operation;
	uint16_t amount;
	bool affectsParty;
public:
	ModifyItemsQuestReward(std::shared_ptr<char>& rawData);
	virtual ~ModifyItemsQuestReward();
	virtual void performAction(QuestTriggerContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_MODIFY_ITEMS_QUEST_REWARDS__