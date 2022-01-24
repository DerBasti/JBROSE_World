#include "StatSufficientCondition.h"
#include "../../../Entities/NPC.h"


StatSufficientCondition::StatSufficientCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);

	abilityType = OperationHandler::fromAIPAbilityTypeId(interpreter.readUInt());
	abilityAmountNecessary = interpreter.readUInt();
	operation = interpreter.readByte() == 0 ? CheckOperationType::BIGGER_EQUAL : CheckOperationType::SMALLER_EQUAL;
}

StatSufficientCondition::~StatSufficientCondition() {

}

bool StatSufficientCondition::isFulfilled(AIContext& context) {
	NPC* sourceNpc = context.getSourceEntity();
	if (sourceNpc->getCombat()->getTarget() != nullptr || context.getDesignatedTarget() == nullptr) {
		return false;
	}
	uint16_t foundAbilityAmount = OperationHandler::getAIPAbilityValueOfEntity(context.getDesignatedTarget(), abilityType);
	return OperationHandler::executeCheckOperation(foundAbilityAmount, abilityAmountNecessary, operation);
}

std::shared_ptr<char> StatSufficientCondition::toPrintable() const {
	char *buffer = new char[0xA0];
	sprintf_s(buffer, 0xA0, "[StatSufficientCondition] AbilityType: %s | Amount necessary: %i | Check Operation: %s", abilityType.getName(), abilityAmountNecessary, operation.getName());
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}