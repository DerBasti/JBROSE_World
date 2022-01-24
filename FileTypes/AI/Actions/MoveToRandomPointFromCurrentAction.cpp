#include "MoveToRandomPointFromCurrentAction.h"
#include "../../../Entities/NPC.h"


MoveToRandomPointFromCurrentPositionAction::MoveToRandomPointFromCurrentPositionAction(std::shared_ptr<char>& rawAction) : AIAction(rawAction) {
	LoadedDataReader interpreter(rawAction.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);

	maxTravelDistance = interpreter.readUInt() * 100.0f;
	travelStance = interpreter.readByte();
}

MoveToRandomPointFromCurrentPositionAction::~MoveToRandomPointFromCurrentPositionAction() {

}

void MoveToRandomPointFromCurrentPositionAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	auto currentPosition = npc->getLocationData()->getMapPosition()->getCurrentPosition();
	npc->getStance()->setStanceById(travelStance);
	npc->getLocationData()->getMapPosition()->setDestinationPositionVisually(std::move(PositionProcessor::generateRandomPointAroundPosition(currentPosition, maxTravelDistance)));
}

std::shared_ptr<char> MoveToRandomPointFromCurrentPositionAction::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[MoveToRandomPointFromCurrentPositionAction] Maximum travel distance: %.2f | Stance: %i", maxTravelDistance, travelStance);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}