#ifndef __ROSE_TELEPORT_PLAYER_QUEST_REWARD__
#define __ROSE_TELEPORT_PLAYER_QUEST_REWARD__

#include "../../QSD.h"
#include "../../../BasicTypes/Position.h"

class TeleportPlayerQuestReward : public QuestReward {
private:
	uint32_t mapId;
	Position position;
	bool includePartyFlag;
public:
	TeleportPlayerQuestReward(std::shared_ptr<char>& rawData);
	virtual ~TeleportPlayerQuestReward();
	virtual void performAction(QuestTriggerContext& context);
	virtual std::shared_ptr<char> toPrintable() const;
};

#endif //__ROSE_TELEPORT_PLAYER_QUEST_REWARD__