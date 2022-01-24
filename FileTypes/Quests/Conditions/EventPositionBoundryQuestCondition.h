#ifndef __ROSE_MODIFY_ITEMS_QUEST_REWARDS__
#define __ROSE_MODIFY_ITEMS_QUEST_REWARDS__

#include "../../QSD.h"
#include "../../../BasicTypes/Position.h"

class EventPositionBoundryQuestCondition : public QuestCondition {
private:
	uint32_t eventId;
	uint32_t xMapTile;
	uint32_t yMapTile;
	uint16_t mapId;
	uint32_t eventHash;
public:
	EventPositionBoundryQuestCondition(std::shared_ptr<char>& rawData);
	virtual ~EventPositionBoundryQuestCondition();
	virtual bool isConditionFulfilled(QuestTriggerContext& context) const;
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_MODIFY_ITEMS_QUEST_REWARDS__