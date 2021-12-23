#include "PositionProcessor.h"
#include "Combat.h"
#include "Entity.h"

class Autocloser {
private:
	std::function<void()> onCloseFunction;
public:
	Autocloser(std::function<void()> func) {
		onCloseFunction = func;
	}
	void setOnCloseFunction(std::function<void()> func) {
		onCloseFunction = func;
	}
	virtual ~Autocloser() {
		if (onCloseFunction) {
			onCloseFunction();
		}
	}
};

PositionProcessor::PositionProcessor(MapPosition* pos, Combat* combat, std::function<uint16_t()> movementSpeedFromEntityMethod) : position(pos) {
	this->movementSpeedFromEntityMethod = movementSpeedFromEntityMethod;
	this->combat = combat;
}

PositionProcessor::~PositionProcessor() {
	position = nullptr;
}

PositionUpdateResult PositionProcessor::processNewPosition() {
	Autocloser closer([&]() {
		timer.updateTimestamp();
	});
	if (combat->isAttackRunning()) {
		return PositionUpdateResult::TARGET_REACHED;
	}
	if (combat->getTarget() != nullptr) {
		Position newDestination(combat->getTarget()->getLocationData()->getMapPosition()->getCurrentPosition());
		logger.logTrace("New Position of destination is: ", newDestination);
		position->setDestinationPosition(std::move(newDestination));
	}

	if (combat->isTargetInReach()) {
		return PositionUpdateResult::TARGET_REACHED;
	}
	if (position->getCurrentPosition() == position->getDestinationPosition()) {
		return PositionUpdateResult::IDLE;
	}
	closer.setOnCloseFunction(nullptr);

	PositionUpdateResult result = PositionUpdateResult::IS_MOVING;
	double timePassed = static_cast<double>(timer.updateTimestamp());
	const float movementSpeed = static_cast<float>(this->movementSpeedFromEntityMethod());
	double distanceMoved = (timePassed * movementSpeed / 1000.0); //MovementSpeed
	double totalDistance = getDistanceToDestination();
	Position newCurrent;
	if (distanceMoved >= totalDistance && combat->getTarget() == nullptr) {
		newCurrent = position->getDestinationPosition();
		result = PositionUpdateResult::TARGET_REACHED;
		if (combat->getEntitySelf()->isPlayer()) {
			logger.logDebug("Target reached.");
		}
	}
	else {
		double ratios[2] = { getDistanceX() / totalDistance, getDistanceY() / totalDistance };

		newCurrent = Position(position->getCurrentPosition().getX() + static_cast<float>(ratios[0] * distanceMoved),
			position->getCurrentPosition().getY() + static_cast<float>(ratios[1] * distanceMoved));
	}
	combat->onMovementUpdate();
	position->setCurrentPosition(std::move(newCurrent));
	if (combat->isTargetInReach()) {
		result = PositionUpdateResult::TARGET_REACHED;
	}

	return result;
}

float PositionProcessor::getDistanceToDestination() const {
	float totalDistance = getDistanceBetweenPoints(position->getCurrentPosition(), position->getDestinationPosition());
	return totalDistance;
}

float PositionProcessor::getDistanceBetweenPoints(const Position& first, const Position& second) {
	float xDist = first.getX() - second.getX();
	float yDist = first.getY() - second.getY();

	return sqrtf((xDist*xDist) + (yDist*yDist));
}

Position PositionProcessor::generateRandomPointAroundPosition(const Position& center, float maxDistanceFromCenter) {
	NumericRandomizer<float> randomizer(-maxDistanceFromCenter, maxDistanceFromCenter);
	Position randomPoint(center.getX() + randomizer.generateRandomValue(), center.getY() + randomizer.generateRandomValue());
	return randomPoint;
}