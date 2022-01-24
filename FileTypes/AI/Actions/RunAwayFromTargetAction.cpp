#include "RunAwayFromTargetAction.h"
#include "../../../Entities/NPC.h"

RunAwayFromTargetAction::RunAwayFromTargetAction(std::shared_ptr<char>& rawData) : AIAction(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	runDistance = reader.readUInt() * 100.0f;
}

RunAwayFromTargetAction::~RunAwayFromTargetAction() {

}

void RunAwayFromTargetAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	if (!npc->getCombat()->hasTarget()) {
		return;
	}
	Position currentPosition = npc->getLocationData()->getMapPosition()->getCurrentPosition();
	Position directionVector = npc->getCombat()->getTarget()->getLocationData()->getMapPosition()->getCurrentPosition() - currentPosition;
	float length = directionVector.toLength();
	float modifier = runDistance / length;

	Position newPosition = Position(currentPosition.getX() + (directionVector.getX() * modifier * -1.0f), currentPosition.getY() + (directionVector.getY() * modifier * -1.0f));
	npc->getLocationData()->getMapPosition()->setDestinationPositionVisually(newPosition);
}

std::shared_ptr<char> RunAwayFromTargetAction::toPrintable() const {
	char *buffer = new char[0x70];
	sprintf_s(buffer, 0x70, "[RunAwayFromTargetAction] Running away for distance: %.2f", runDistance);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}