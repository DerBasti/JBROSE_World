#ifndef __ROSE_SN_QUEST_CONDITION__
#define __ROSE_SN_QUEST_CONDITION__

#include "../../QSD.h"

class QuestFlagValidationQuestCondition : public QuestCondition {
private:
	uint16_t questFlagPosition;
	bool flagState;
public:
	QuestFlagValidationQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~QuestFlagValidationQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_SN_QUEST_CONDITION__