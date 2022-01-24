#include "CheckVariableQuestCondition.h"
#include "../../../WorldClient.h"

CheckVariableQuestCondition::CheckVariableQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	amountOfChecks = reader.readUInt();
	for (uint32_t i = 0; i < amountOfChecks; i++) {
		checks.push_back(new QuestDataCheck(reader));
	}
}

CheckVariableQuestCondition::~CheckVariableQuestCondition() {
	for (auto it = checks.begin(); it != checks.end(); it++) {
		delete (*it);
	}
	checks.clear();
}

bool CheckVariableQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	if (!context.getTriggerEntity()->isPlayer()) {
		return false;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	for (auto it = checks.begin(); it != checks.end(); it++) {
		QuestDataCheck *check = *it;
		uint16_t valueToCheck = player->getQuestJournal()->getQuestVariableByTypeAndSlot(context.getSelectedQuest(), check->getVariableType(), check->getVariableNumber());
		if (!OperationHandler::executeCheckOperation(valueToCheck, check->getValue(), check->getCheckOperationType())) {
			return false;
		}
	}
	return true;
}

std::shared_ptr<char> CheckVariableQuestCondition::toPrintable() const {
	char *buffer = new char[0x350];
	sprintf_s(buffer, 0x350, "[CheckVariableQuestCondition] Amount of Checks: %zi", checks.size());
	for (auto it = checks.begin(); it != checks.end(); it++) {
		QuestDataCheck* check = *it;
		uint32_t len = (uint32_t)strlen(buffer);
		sprintf_s(&buffer[len], 0x350 - len, " | VariableType: 0x%x, Slot: %i, CompareValue: %i, Operation: %s", check->getVariableType(), check->getVariableNumber(), check->getValue(), check->getCheckOperationType().getName());
	}
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}