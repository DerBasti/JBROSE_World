#ifndef __ROSE_POSITION_PROCESSOR__
#define __ROSE_POSITION_PROCESSOR__

#include <cstdint>
#include "Position.h"

enum class PositionUpdateResult : uint16_t {
	IDLE = 0,
	IS_MOVING,
	TARGET_REACHED,
	COMBAT_TARGET_REACHED
};

class PositionProcessor {
private:
	class Combat* combat;
	ROSEThreadedLogger logger;
	MapPosition* position;
	Timer timer;
	bool forceUpdateRequiredFlag;
	std::function<uint16_t()> movementSpeedFromEntityMethod;

	__inline float getDistanceX() const {
		return position->getDestinationPosition().getX() - position->getCurrentPosition().getX();
	}
	__inline float getDistanceY() const {
		return position->getDestinationPosition().getY() - position->getCurrentPosition().getY();
	}
public:
	PositionProcessor(MapPosition* pos, Combat* combat, std::function<uint16_t()> movementSpeedFromEntityMethod);
	virtual ~PositionProcessor();

	virtual void onNewDestination() {
	}

	PositionUpdateResult processNewPosition();
	float getDistanceToDestination() const;
	static Position generateRandomPointAroundPosition(const Position& center, float maxDistanceFromCenter);
	static float getDistanceBetweenPoints(const Position& first, const Position& second);

	__inline void setUpdateRequiredFlag(bool flag) {
		forceUpdateRequiredFlag = flag;
	}
	__inline bool isUpdateRequired() const {
		return forceUpdateRequiredFlag;
	}
};

#endif //__ROSE_POSITION_PROCESSOR__