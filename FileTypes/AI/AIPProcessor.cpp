#include "AIPProcessor.h"
#include "../AIP.h"
#include "..\..\Entities\NPC.h"

AIPProcessor::AIPProcessor(NPC* npc) {
	this->npc = npc;
}

AIPProcessor::~AIPProcessor() {

}

bool AIPProcessor::triggerProcess(AIEvent& eventType) {
	return triggerProcess(eventType, nullptr);
}

bool AIPProcessor::triggerProcess(AIEvent& eventType, Entity* designatedTarget) {
	auto aiProtocol = npc->getArtificialIntelligence();
	if (!aiProtocol || (triggerTimer.getPassedTimeInMillis() < aiProtocol->getCheckingIntervallInMilliseconds() && eventType == AIEvent::IDLE)) {
		return false;
	}
	triggerTimer.updateTimestamp();
	AIState* currentState = npc->getArtificialIntelligence()->getRecordsForEvent(eventType);
	AIContext context(npc, eventType, designatedTarget);
	for (uint32_t i = 0; i < currentState->getAmountOfRecords(); i++) {
		auto record = currentState->getRecord(i);
		if (record->conditionsFulfilled(context)) {
			record->performAction(context);
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, const AIEvent& event) {
	out << (int)event;
	return out;
}