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
	uint32_t rolled = rand() % 100;
	return percentage <= rolled;
}