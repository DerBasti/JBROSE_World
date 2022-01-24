#include "RandomPercentageCondition.h"
#include "../../../Entities/NPC.h"

RandomPercentageCondition::RandomPercentageCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader interpreter(rawData.get());
	interpreter.skipBytes(AIDataBlock::DEFAULT_HEADER_LENGTH);
	percentage = interpreter.readByte();
}
RandomPercentageCondition::~RandomPercentageCondition() {

}
bool RandomPercentageCondition::isFulfilled(AIContext& context) {
	NumericRandomizer<uint32_t> randomizer(0, 100);
	uint32_t rolled = randomizer.generateRandomValue();
	return rolled <= percentage;
}

std::shared_ptr<char> RandomPercentageCondition::toPrintable() const {
	char *buffer = new char[0x90];
	sprintf_s(buffer, 0x90, "[RandomPercentageCondition] Percentage: %i", percentage);
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}