#include "CompareAbilityValueOfEntityCondition.h"
#include "../../../Entities/NPC.h"

CompareAbilityValueOfEntityCondition::CompareAbilityValueOfEntityCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader reader(datablock.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	abilityType = OperationHandler::fromAbilityTypeId(reader.readByte());
	operation = reader.readByte() == 0 ? OperationType::BIGGER : OperationType::SMALLER;
}

bool CompareAbilityValueOfEntityCondition::isFulfilled(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (sourceNpc->getCombat()->getTarget() == nullptr || context.getDesignatedTarget() == nullptr) {
		return false;
	}
	uint16_t currentValue = OperationHandler::getAbilityValueOfEntity(sourceNpc->getCombat()->getTarget(), abilityType);
	uint16_t designatedTargetValue = OperationHandler::getAbilityValueOfEntity(context.getDesignatedTarget(), abilityType);

	return OperationHandler::executeCheckOperation(currentValue, designatedTargetValue, operation);
}