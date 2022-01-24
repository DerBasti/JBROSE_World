#include "CheckDistanceToTargetCondition.h"
#include "../../../Entities/Monster.h"

CheckDistanceToTargetCondition::CheckDistanceToTargetCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader reader(datablock.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	neededDistance = reader.readUInt() * 100.0f;
	operation = reader.readByte() == 0 ? CheckOperationType::BIGGER_EQUAL : CheckOperationType::SMALLER_EQUAL;
}

CheckDistanceToTargetCondition::CheckDistanceToTargetCondition(uint32_t operationCode, uint32_t length, const char* contextData) : AICondition(operationCode, length, contextData) {

}

bool CheckDistanceToTargetCondition::isFulfilled(AIContext& context) {
	auto npc = context.getSourceEntity();
	float currentDistance = PositionProcessor::getDistanceBetweenPoints(npc->getLocationData()->getMapPosition()->getSourcePosition(), npc->getLocationData()->getMapPosition()->getCurrentPosition());
	return OperationHandler::executeCheckOperation(currentDistance, neededDistance, operation);
}

std::shared_ptr<char> CheckDistanceToTargetCondition::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[CheckDistanceToTargetCondition] Distance: %.2f | Check operation: %s", neededDistance, operation.getName());
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}