#include "AIVariableCheckQuestCondition.h"
#include "../../../Entities/NPC.h"

AIVariableCheckQuestCondition::AIVariableCheckQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	entityType = static_cast<uint8_t>(reader.readUShort());
	aiVariableSlot = reader.readUShort();
	aiVariableSlot &= 0xFF;
	value = reader.readUInt();
	operation = OperationHandler::fromCheckOperationTypeId(reader.readByte());
}
AIVariableCheckQuestCondition::~AIVariableCheckQuestCondition() {

}

bool AIVariableCheckQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	if (entityType != 0) {
		logger.logWarn("Different EntityType than NPC was selected for AIVariable: %i", entityType);
		return false;
	}
	if (!context.getSelectedEntity() || !context.getSelectedEntity()->isNPC()) {
		logger.logWarn("NPC should've been selected beforehand! Trigger entity: ", context.getTriggerEntity()->getName(), ", QuestHash: 0x", ROSELogger::asHex(getQuestHash()));
		return false;
	}
	NPC* npc = dynamic_cast<NPC*>(context.getTriggerEntity());
	uint32_t currentAiValue = npc->getAIVariableBySlot(aiVariableSlot);
	return OperationHandler::executeCheckOperation(currentAiValue, value, operation);
}

std::shared_ptr<char> AIVariableCheckQuestCondition::toPrintable() const {
	char *buffer = new char[0x100];
	sprintf_s(buffer, 0x100, "[AIVariableCheckQuestCondition] EntityType Selection: %i, AiVariableSlot: %i, Value to check for: %i, Operation: %s", entityType, aiVariableSlot, value, operation.getName());
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}