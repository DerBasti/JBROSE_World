#include "MoveToRandomPointFromSpawnAction.h"
#include "../../../Entities/NPC.h"


MoveToRandomPointFromSpawnPositionAction::MoveToRandomPointFromSpawnPositionAction(std::shared_ptr<char>& rawAction) : AIAction(rawAction) {
	LoadedDataReader interpreter(rawAction.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);

	maxTravelDistance = interpreter.readUInt() * 100.0f;
	
	travelStance = interpreter.readByte();
}

MoveToRandomPointFromSpawnPositionAction::~MoveToRandomPointFromSpawnPositionAction() {

}

void MoveToRandomPointFromSpawnPositionAction::performAction(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	auto sourcePosition = npc->getLocationData()->getMapPosition()->getSourcePosition();
	npc->getStance()->setStanceById(travelStance);
	npc->getLocationData()->getMapPosition()->setDestinationPositionVisually(std::move(PositionProcessor::generateRandomPointAroundPosition(sourcePosition, maxTravelDistance)));
}