#include "MapTime.h"
#include "../FileTypes/STB.h"

MapTime::MapTime() {
	totalDayTimeInSeconds = morningStartTimeInSeconds = noonStartTimeInSeconds = 0;
	eveningStartTimeInSeconds = nightStartTimeInSeconds = 0;
}

MapTime::MapTime(ZoneSTBFile* file, uint16_t mapId) {
	totalDayTimeInSeconds = file->getTotalDayTimeInSeconds(mapId) * 10;
	mapTime.setDurationForWrappingInMillis(totalDayTimeInSeconds * 1000);

	morningStartTimeInSeconds = file->getMorningStartTimeInSeconds(mapId) * 10;
	noonStartTimeInSeconds = file->getNoonStartTimeInSeconds(mapId) * 10;
	eveningStartTimeInSeconds = file->getEveningStartTimeInSeconds(mapId) * 10;
	nightStartTimeInSeconds = file->getNightStartTimeInSeconds(mapId) * 10;
}

MapTimeType MapTime::getCurrentDayTimeType() const {
	uint64_t currentTimeInSeconds = getCurrentTimeInSeconds();
	if (currentTimeInSeconds >= nightStartTimeInSeconds) {
		return MapTimeType::NIGHT_TIME;
	}
	else if (currentTimeInSeconds >= eveningStartTimeInSeconds) {
		return MapTimeType::EVENING_TIME;
	}
	else if (currentTimeInSeconds >= noonStartTimeInSeconds) {
		return MapTimeType::NOON_TIME;
	}
	return MapTimeType::MORNING_TIME;
}

const char* MapTime::mapTimeAsString(const MapTimeType& time) {
	switch (time) {
		case MapTimeType::MORNING_TIME:
			return "Morning";
		case MapTimeType::NOON_TIME:
			return "Noon";
		case MapTimeType::EVENING_TIME:
			return "Evening";
		case MapTimeType::NIGHT_TIME:
			return "Night";
	}
	return "Unknown";
}