#include "DistanceFromSpawnCondition.h"
#include "..\..\..\Entities\NPC.h"

DistanceFromSpawnCondition::DistanceFromSpawnCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);
	allowedMaximumDistance = interpreter.readUInt() * 100.0f;
	operation = interpreter.readByte() == 0 ? OperationType::BIGGER_EQUAL : OperationType::SMALLER_EQUAL;
}

DistanceFromSpawnCondition::~DistanceFromSpawnCondition() {

}

bool DistanceFromSpawnCondition::isFulfilled(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), npc->getLocationData()->getMapPosition()->getSourcePosition());
	return OperationHandler::executeCheckOperation(distance, allowedMaximumDistance, operation);
}