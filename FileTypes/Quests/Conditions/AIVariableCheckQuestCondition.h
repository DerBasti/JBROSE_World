#ifndef __ROSE_AI_VARIABLE_CHECK_QUEST_CONDITION__
#define __ROSE_AI_VARIABLE_CHECK_QUEST_CONDITION__

#include "../../QSD.h"

class AIVariableCheckQuestCondition : public QuestCondition {
private:
	uint8_t entityType;
	uint16_t aiVariableSlot;
	uint32_t value;
	CheckOperationType operation;
public:
	AIVariableCheckQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~AIVariableCheckQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif