#ifndef __ROSE_MAP_TIME__
#define __ROSE_MAP_TIME__

#include <inttypes.h>
#include "../../JBROSE_Common/Timer.h"

enum class MapTimeType : uint8_t {
	MORNING_TIME,
	NOON_TIME,
	EVENING_TIME,
	NIGHT_TIME
};

class MapTime {
private:
	uint16_t totalDayTimeInSeconds;
	uint16_t morningStartTimeInSeconds;
	uint16_t noonStartTimeInSeconds;
	uint16_t eveningStartTimeInSeconds;
	uint16_t nightStartTimeInSeconds;

	WrappingTimer mapTime;
public:
	MapTime();
	MapTime(class ZoneSTBFile* file, uint16_t mapId);
	virtual ~MapTime() {}
	MapTimeType getCurrentDayTimeType() const;

	static const char* mapTimeAsString(const MapTimeType& time);

	__inline uint16_t getTotalDayTimeInSeconds() const {
		return totalDayTimeInSeconds;
	}
	__inline uint16_t getMorningStartTimeInSeconds() const {
		return morningStartTimeInSeconds;
	}
	__inline uint16_t getNoonStartTimeInSeconds() const {
		return noonStartTimeInSeconds;
	}
	__inline uint16_t getEveningStartTimeInSeconds() const {
		return eveningStartTimeInSeconds;
	}
	__inline uint16_t getNightStartTimeInSeconds() const {
		return nightStartTimeInSeconds;
	}
	__inline uint64_t getCurrentTimeInSeconds() const {
		return mapTime.getPassedTimeInMillis() / 1000;
	}
	__inline uint64_t updateTime() {
		return mapTime.updateTimestamp();
	}
};

#endif //