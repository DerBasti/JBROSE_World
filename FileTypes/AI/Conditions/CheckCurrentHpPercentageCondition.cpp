#include "CheckCurrentHpPercentageCondition.h"
#include "../../../Entities/NPC.h"

CheckCurrentHpPercentageCondition::CheckCurrentHpPercentageCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader interpreter(datablock.get());
	interpreter.skipBytes(DEFAULT_HEADER_LENGTH);

	hpInPercent = interpreter.readUInt();
	operation = interpreter.readByte() == 0 ? OperationType::BIGGER_EQUAL : OperationType::SMALLER_EQUAL;
}

CheckCurrentHpPercentageCondition::~CheckCurrentHpPercentageCondition() {

}

bool CheckCurrentHpPercentageCondition::isFulfilled(AIContext& context) {
	uint32_t percentHp = static_cast<uint32_t>(context.getSourceEntity()->getStats()->getCurrentHp() * 100 / context.getSourceEntity()->getStats()->getMaxHp());
	return OperationHandler::executeCheckOperation(percentHp, hpInPercent, operation);
}