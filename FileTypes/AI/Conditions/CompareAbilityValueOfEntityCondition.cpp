#include "CompareAbilityValueOfEntityCondition.h"
#include "../../../Entities/NPC.h"

CompareAbilityValueOfEntityCondition::CompareAbilityValueOfEntityCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader reader(datablock.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);
	abilityType = OperationHandler::fromAIPAbilityTypeId(reader.readByte());
	operation = reader.readByte() == 0 ? CheckOperationType::BIGGER : CheckOperationType::SMALLER;
}

bool CompareAbilityValueOfEntityCondition::isFulfilled(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (sourceNpc->getCombat()->getTarget() == nullptr || context.getDesignatedTarget() == nullptr) {
		return false;
	}
	uint16_t currentValue = OperationHandler::getAIPAbilityValueOfEntity(sourceNpc->getCombat()->getTarget(), abilityType);
	uint16_t designatedTargetValue = OperationHandler::getAIPAbilityValueOfEntity(context.getDesignatedTarget(), abilityType);

	return OperationHandler::executeCheckOperation(currentValue, designatedTargetValue, operation);
}