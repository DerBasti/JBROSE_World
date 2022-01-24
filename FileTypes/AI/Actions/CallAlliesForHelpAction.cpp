#include "CallAlliesForHelpAction.h"
#include "../../../Entities/NPC.h"
#include "../../../Entities/Monster.h"
#include "../../../Map/MapSector.h"

CallAlliesForHelpAction::CallAlliesForHelpAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	callDistance = reader.readUInt() * 100.0f;
	amountOfAllies = reader.readUInt();
}

CallAlliesForHelpAction::~CallAlliesForHelpAction() {

}

void CallAlliesForHelpAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	uint32_t alliesHelping = 0;
	auto currentSector = npc->getLocationData()->getCurrentMapSector();
	for (auto entityPair : currentSector->getAllEntitiesOfSector()) {
		Entity* possibleHelper = entityPair.second;
		if (!possibleHelper || !possibleHelper->isIngame() || !possibleHelper->isMonster() || possibleHelper == npc) {
			continue;
		}
		float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), possibleHelper->getLocationData()->getMapPosition()->getCurrentPosition());
		Monster* helper = dynamic_cast<Monster*>(possibleHelper);
		if (distance <= callDistance && !helper->getCombat()->hasTarget()) {
			alliesHelping++;
			helper->getCombat()->setTarget(npc->getCombat()->getTarget(), CombatType::BASIC_ATTACK);
			if (alliesHelping >= amountOfAllies) {
				return;
			}
		}
	}
}

std::shared_ptr<char> CallAlliesForHelpAction::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[CallAlliesForHelpAction] Calling distance: %.2f | Amount wanted: %i", callDistance, amountOfAllies);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}