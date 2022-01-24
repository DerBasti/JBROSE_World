#ifndef __ROSE_VALIDATE_CURRENT_POSITION_QUEST_CONDITION__
#define __ROSE_VALIDATE_CURRENT_POSITION_QUEST_CONDITION__

#include "../../QSD.h"
#include "../../../BasicTypes/Position.h"

class ValidateCurrentPositionQuestCondition : public QuestCondition {
private:
	uint8_t mapId;
	Position centerPosition;
	float maximumAllowedDistanceFromCenter;
public:
	ValidateCurrentPositionQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~ValidateCurrentPositionQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_VALIDATE_CURRENT_POSITION_QUEST_CONDITION__