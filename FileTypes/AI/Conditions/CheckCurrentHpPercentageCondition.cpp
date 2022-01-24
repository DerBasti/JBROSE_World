#include "CheckCurrentHpPercentageCondition.h"
#include "../../../Entities/NPC.h"

CheckCurrentHpPercentageCondition::CheckCurrentHpPercentageCondition(std::shared_ptr<char>& datablock) : AICondition(datablock) {
	LoadedDataReader interpreter(datablock.get());
	interpreter.skipBytes(DEFAULT_HEADER_LENGTH);

	hpInPercent = interpreter.readUInt();
	operation = interpreter.readByte() == 0 ? CheckOperationType::BIGGER_EQUAL : CheckOperationType::SMALLER_EQUAL;
}

CheckCurrentHpPercentageCondition::~CheckCurrentHpPercentageCondition() {

}

bool CheckCurrentHpPercentageCondition::isFulfilled(AIContext& context) {
	uint32_t percentHp = static_cast<uint32_t>(context.getSourceEntity()->getStats()->getCurrentHp() * 100 / context.getSourceEntity()->getStats()->getMaxHp());
	return OperationHandler::executeCheckOperation(percentHp, hpInPercent, operation);
}

std::shared_ptr<char> CheckCurrentHpPercentageCondition::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[CheckCurrentHpPercentageCondition] Hp Percentage: %i | Check operation: %s", hpInPercent, operation.getName());
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}