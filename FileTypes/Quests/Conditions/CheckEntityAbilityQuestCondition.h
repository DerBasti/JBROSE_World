#ifndef __ROSE_CHECK_ENTITY_ABILITY_QUESTION_CONDITION__
#define __ROSE_CHECK_ENTITY_ABILITY_QUESTION_CONDITION__

#include "../../QSD.h"

class CheckEntityAbilityQuestCondition : public QuestCondition {
private:
	uint32_t amountOfChecks;
	QuestAbilityTypeCheck* checkTypes;
public:
	CheckEntityAbilityQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~CheckEntityAbilityQuestCondition();
	virtual bool isConditionFulfilled() const;
};

#endif //__ROSE_CHECK_ENTITY_ABILITY_QUESTION_CONDITION__