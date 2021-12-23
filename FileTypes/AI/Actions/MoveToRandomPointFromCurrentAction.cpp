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