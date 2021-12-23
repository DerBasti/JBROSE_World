#include "CheckDistanceToTargetCondition.h"
#include "../../../Entities/Monster.h"

CheckDistanceToTargetCondition::CheckDistanceToTargetCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader reader(datablock.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	neededDistance = reader.readUInt() * 100;
	operation = reader.readByte() == 0 ? OperationType::BIGGER_EQUAL : OperationType::SMALLER_EQUAL;
}

CheckDistanceToTargetCondition::CheckDistanceToTargetCondition(uint32_t operationCode, uint32_t length, const char* contextData) : AICondition(operationCode, length, contextData) {

}

bool CheckDistanceToTargetCondition::isFulfilled(AIContext& context) {
	auto npc = context.getSourceEntity();
	float currentDistance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getSourcePosition(), npc->getLocationData()->getMapPosition()->getCurrentPosition());
	return OperationHandler::executeCheckOperation(currentDistance, neededDistance, operation);
}