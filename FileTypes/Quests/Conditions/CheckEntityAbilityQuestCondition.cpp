#include "CheckEntityAbilityQuestCondition.h"
#include "../../../WorldClient.h"
#include "../../../BasicTypes/EntityStatHandler.h"

CheckEntityAbilityQuestCondition::CheckEntityAbilityQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	amountOfChecks = reader.readUInt();
	checkTypes = new QuestAbilityTypeCheck[amountOfChecks];
	for (uint32_t i = 0; i < amountOfChecks; i++) {
		checkTypes[i] = QuestAbilityTypeCheck(reader);
	}
}

CheckEntityAbilityQuestCondition::~CheckEntityAbilityQuestCondition() {
	if (checkTypes) {
		delete[] checkTypes;
		checkTypes = nullptr;
	}
}

bool CheckEntityAbilityQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	if (!context.getTriggerEntity()->isPlayer()) {
		logger.logWarn("NPC '", context.getTriggerEntity()->getName(), "' tried triggering CheckEntityAbilityQuestCondition.");
		return false;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	for (uint32_t i = 0; i < amountOfChecks; i++) {
		QuestAbilityTypeCheck check = checkTypes[i];
		auto getterAndSetter = EntityStatHandler::getNumericGetterAndSetterOfPlayerByEntityAbilityType<uint32_t>(player, check.getEntityAbilityType());
		if (!OperationHandler::executeCheckOperation(getterAndSetter.get(), check.getValue(), check.getCheckOperationType())) {
			return false;
		}
	}
	return true;
}

std::shared_ptr<char> CheckEntityAbilityQuestCondition::toPrintable() const {
	char *buffer = new char[0x350];
	sprintf_s(buffer, 0x350, "[CheckEntityAbilityQuestCondition] Amount of Checks: %i", amountOfChecks);
	for (uint32_t i=0;i<amountOfChecks;i++) {
		QuestAbilityTypeCheck check = checkTypes[i];
		uint32_t len = (uint32_t)strlen(buffer);
		sprintf_s(&buffer[len], 0x350 - len, " | AbilityType: %s (%i), Amount %i, Operation: %s", check.getEntityAbilityType().getName(), check.getEntityAbilityType().getTypeId(), check.getValue(), check.getCheckOperationType().getName());
	}
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}