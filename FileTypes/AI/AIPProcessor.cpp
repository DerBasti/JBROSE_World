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
	if (!aiProtocol) {
		return false;
	}
	switch (eventType) {
		case AIEvent::IDLE:
			if (triggerTimer.getPassedTimeInMillis() < aiProtocol->getCheckingIntervallInMilliseconds()) {
				return false;
			}
			triggerTimer.updateTimestamp();
		break;
		case AIEvent::DAMAGED:
		{
			NumericRandomizer<uint32_t> randomizer(0, 100);
			if (npc->getCombat()->hasTarget() && randomizer.generateRandomValue() > aiProtocol->getChanceForTriggerOnDamaged()) {
				return false;
			}
			logger.logDebug("OnDamaged Trigger passed chance-test");
		}
		break;
	}
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

std::wostream& operator<<(std::wostream& out, const AIEvent& event) {
	out << (int)event;
	return out;
}