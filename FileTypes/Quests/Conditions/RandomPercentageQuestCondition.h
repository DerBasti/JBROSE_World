#ifndef __ROSE_RANDOM_PERCENTAGE_QUEST_CONDITION__
#define __ROSE_RANDOM_PERCENTAGE_QUEST_CONDITION__

#include "../../QSD.h"

class RandomPercentageQuestCondition : public QuestCondition {
private:
	uint8_t lowerBoundPercentage;
	uint8_t upperBoundPercentage;
public:
	RandomPercentageQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~RandomPercentageQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_RANDOM_PERCENTAGE_QUEST_CONDITION__