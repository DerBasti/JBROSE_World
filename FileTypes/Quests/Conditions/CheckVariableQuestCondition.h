#ifndef __ROSE_CHECK_VARIABLE_QUEST_CONDITION__
#define __ROSE_CHECK_VARIABLE_QUEST_CONDITION__

#include "../../QSD.h"

class CheckVariableQuestCondition : public QuestCondition {
private:
	uint32_t amountOfChecks;
	std::list<QuestDataCheck*> checks;
public:
	CheckVariableQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~CheckVariableQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_CHECK_VARIABLE_QUEST_CONDITION__