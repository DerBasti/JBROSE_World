#include "DistanceFromSpawnCondition.h"
#include "..\..\..\Entities\NPC.h"

DistanceFromSpawnCondition::DistanceFromSpawnCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);
	allowedMaximumDistance = interpreter.readUInt() * 100.0f;
	operation = interpreter.readByte() == 0 ? CheckOperationType::BIGGER_EQUAL : CheckOperationType::SMALLER_EQUAL;
}

DistanceFromSpawnCondition::~DistanceFromSpawnCondition() {

}

bool DistanceFromSpawnCondition::isFulfilled(AIContext& context) {
	NPC* npc = context.getSourceEntity();
	float distance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getCurrentPosition(), npc->getLocationData()->getMapPosition()->getSourcePosition());
	return OperationHandler::executeCheckOperation(distance, allowedMaximumDistance, operation);
}

std::shared_ptr<char> DistanceFromSpawnCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[DistanceFromSpawnCondition] Allowed maximum distance: %.2f | Check operation: %s", allowedMaximumDistance, operation.getName());
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}