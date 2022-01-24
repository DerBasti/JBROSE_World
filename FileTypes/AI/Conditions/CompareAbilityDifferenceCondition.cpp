#include "CompareAbilityDifferenceCondition.h"
#include "../../../Entities/NPC.h"
#include "../../../WorldClient.h"

CompareAbilityDifferenceCondition::CompareAbilityDifferenceCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	abilityType = OperationHandler::fromAIPAbilityTypeId(reader.readUInt());
	allowedDifference = reader.readUInt();
	operation = reader.readByte() == 1 ? CheckOperationType::SMALLER_EQUAL : CheckOperationType::BIGGER_EQUAL;
}

CompareAbilityDifferenceCondition::~CompareAbilityDifferenceCondition() {

}

bool CompareAbilityDifferenceCondition::isFulfilled(AIContext& context) {
	Entity* target = context.getSourceEntity()->getCombat()->getTarget();
	if (target == nullptr) {
		return false;
	}
	int32_t value = (int32_t)OperationHandler::getAIPAbilityValueOfEntity(target, abilityType);
	return OperationHandler::executeCheckOperation(value, allowedDifference, operation);
}

std::shared_ptr<char> CompareAbilityDifferenceCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[CompareAbilityDifferenceCondition] AbilityType: %s | Allowed Difference: %i | Check operation: %s", abilityType.getName(), allowedDifference, operation.getName());
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}