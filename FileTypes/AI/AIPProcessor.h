#ifndef __ROSE_AIP_PROCESSOR__
#define __ROSE_AIP_PROCESSOR__

#include "..\..\..\JBROSE_Common\Timer.h"

class AIPProcessor {
private:
	class NPC* npc;
	Timer triggerTimer;
public:
	AIPProcessor(class NPC* npc);
	virtual ~AIPProcessor();

	bool triggerProcess(enum class AIEvent& eventType);
	bool triggerProcess(enum class AIEvent& eventType, class Entity* designatedTarget);
	__inline void updateTriggerTimer() {
		triggerTimer.updateTimestamp();
	}
};

std::ostream& operator<<(std::ostream& out, const AIEvent& event);
std::wostream& operator<<(std::wostream& out, const AIEvent& event);

#endif //__ROSE_AIP_PROCESSOR__