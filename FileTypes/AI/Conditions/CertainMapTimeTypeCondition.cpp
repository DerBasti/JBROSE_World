#include "CertainMapTimeTypeCondition.h"
#include "../../../Entities/NPC.h"
#include "../../../Map/Map.h"

CertainMapTimeTypeCondition::CertainMapTimeTypeCondition(std::shared_ptr<char>& rawData) : AICondition(rawData) {
	LoadedDataReader reader(rawData.get());
	reader.skipBytes(DEFAULT_HEADER_LENGTH);

	timeType = static_cast<MapTimeType>(reader.readByte());
}

CertainMapTimeTypeCondition::~CertainMapTimeTypeCondition() {

}

bool CertainMapTimeTypeCondition::isFulfilled(AIContext& context) {
	return timeType == context.getSourceEntity()->getLocationData()->getMap()->getMapTime().getCurrentDayTimeType();
}

std::shared_ptr<char> CertainMapTimeTypeCondition::toPrintable() const {
	char *buffer = new char[0x80];
	sprintf_s(buffer, 0x80, "[CertainMapTimeTypeCondition] Checking for time type: %s", MapTime::mapTimeAsString(timeType));
	std::shared_ptr<char> result = std::shared_ptr<char>(buffer, std::default_delete<char[]>());
	return result;
}