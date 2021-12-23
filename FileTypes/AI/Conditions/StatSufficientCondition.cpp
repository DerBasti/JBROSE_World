#include "StatSufficientCondition.h"
#include "../../../Entities/NPC.h"


StatSufficientCondition::StatSufficientCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);

	abilityType = OperationHandler::fromAbilityTypeId(interpreter.readUInt());
	abilityAmountNecessary = interpreter.readUInt();
	operation = interpreter.readByte() == 0 ? OperationType::BIGGER_EQUAL : OperationType::SMALLER_EQUAL;
}

StatSufficientCondition::~StatSufficientCondition() {

}

bool StatSufficientCondition::isFulfilled(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (sourceNpc->getCombat()->getTarget() != nullptr || context.getDesignatedTarget() == nullptr) {
		return false;
	}
	uint16_t foundAbilityAmount = OperationHandler::getAbilityValueOfEntity(context.getDesignatedTarget(), abilityType);
	return OperationHandler::executeCheckOperation(foundAbilityAmount, abilityAmountNecessary, operation);
}