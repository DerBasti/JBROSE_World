#include "RandomPercentageQuestCondition.h"
#include "../../../../JBROSE_Common/Randomizer.h"

RandomPercentageQuestCondition::RandomPercentageQuestCondition(std::shared_ptr<char>& rawData) : QuestCondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(HEADER_LENGTH);

	lowerBoundPercentage = reader.readByte();
	upperBoundPercentage = reader.readByte();
}
RandomPercentageQuestCondition::~RandomPercentageQuestCondition() {

}
bool RandomPercentageQuestCondition::isConditionFulfilled(QuestTriggerContext& context) const {
	NumericRandomizer<uint8_t> randomizer(0, 100);
	uint8_t drawnValue = randomizer.generateRandomValue();
	return drawnValue >= lowerBoundPercentage && drawnValue <= upperBoundPercentage;
}

std::shared_ptr<char> RandomPercentageQuestCondition::toPrintable() const {
	char *buffer = new char[0x70];
	sprintf_s(buffer, 0x70, "[RandomPercentageQuestCondition] Randomly drawn percentage between boundries: [%i,%i]", lowerBoundPercentage, upperBoundPercentage);
	return std::shared_ptr<char>(buffer, std::default_delete<char[]>());
}