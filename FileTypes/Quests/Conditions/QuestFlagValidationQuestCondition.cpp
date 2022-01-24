#include "QuestFlagValidationQuestCondition.h"
#include "../../../WorldClient.h"

QuestFlagValidationQuestCondition::QuestFlagValidationQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	questFlagPosition = reader.readUShort();
	flagState = (reader.readUShort() & 1);
}

QuestFlagValidationQuestCondition::~QuestFlagValidationQuestCondition() {

}

bool QuestFlagValidationQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	if (!context.getTriggerEntity()->isPlayer()) {
		logger.logWarn("NPC '", context.getTriggerEntity()->getName(), "' tried to trigger QuestFlagValidationQuestCondition");
		return false;
	}
	Player* player = dynamic_cast<Player*>(context.getTriggerEntity());
	return (player->getQuestJournal()->isGlobalFlagSet(questFlagPosition) == flagState);
}

std::shared_ptr<char> QuestFlagValidationQuestCondition::toPrintable() const {
	char *buffer = new char[0x70];
	sprintf_s(buffer, 0x70, "[QuestFlagValidationQuestCondition] Quest Flag: %i (Position: %i | BitPosition: %i), Flag needs to be: %i", questFlagPosition, (questFlagPosition/8), (questFlagPosition%8), flagState);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}