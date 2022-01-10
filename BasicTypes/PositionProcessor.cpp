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
		return combat->isTargetInReach() ? PositionUpdateResult::COMBAT_TARGET_REACHED : PositionUpdateResult::TARGET_REACHED;
	}
	if (combat->getTarget() != nullptr) {
		Position newDestination(combat->getTarget()->getLocationData()->getMapPosition()->getCurrentPosition());
		logger.logTrace("New Position of destination is: ", newDestination);
		position->setDestinationPosition(std::move(newDestination));
	}

	if (combat->isTargetInReach()) {
		return combat->isAttackingEnemy() ? PositionUpdateResult::COMBAT_TARGET_REACHED : PositionUpdateResult::TARGET_REACHED;
	}
	if (position->getCurrentPosition() == position->getDestinationPosition()) {
		return PositionUpdateResult::IDLE;
	}
	closer.setOnCloseFunction(nullptr);

	PositionUpdateResult result = PositionUpdateResult::IS_MOVING;
	float movementModifier = combat->getEntitySelf()->getStance()->isWalking() ? 1.0f : ((this->movementSpeedFromEntityMethod() + 180.0f) / 600.0f);
	const float movementSpeed = movementModifier * this->movementSpeedFromEntityMethod();
	float timePassed = static_cast<float>(timer.updateTimestamp());
	float distanceMoved = (timePassed * movementSpeed / 1000.0f); //MovementSpeed
	float totalDistance = getDistanceToDestination();
	Position newCurrent;
	if (distanceMoved >= totalDistance && combat->getTarget() == nullptr) {
		newCurrent = position->getDestinationPosition();
		result = PositionUpdateResult::TARGET_REACHED;
		if (combat->getEntitySelf()->isPlayer()) {
			logger.logDebug("Target reached.");
		}
	}
	else {
		float ratios[2] = { getDistanceX() / totalDistance, getDistanceY() / totalDistance };

		newCurrent = Position(position->getCurrentPosition().getX() + (ratios[0] * distanceMoved),
			position->getCurrentPosition().getY() + (ratios[1] * distanceMoved));

		if (combat->getEntitySelf()->isPlayer()) {
			logger.logDebug("Position: ", newCurrent);
		}
	}
	combat->onMovementUpdate();
	position->setCurrentPosition(std::move(newCurrent));
	if (combat->isTargetInReach()) {
		result = combat->isAttackingEnemy() ? PositionUpdateResult::COMBAT_TARGET_REACHED : PositionUpdateResult::TARGET_REACHED;
	}

	return result;
}

float PositionProcessor::getDistanceToDestination() const {
	float totalDistance = getDistanceBetweenPoints(position->getCurrentPosition(), position->getDestinationPosition());
	return totalDistance;
}

float PositionProcessor::getDistanceBetweenPoints(const Position& first, const Position& second) {
	Position distanceDifference = first - second;
	return distanceDifference.toLength();
}

Position PositionProcessor::generateRandomPointAroundPosition(const Position& center, float maxDistanceFromCenter) {
	NumericRandomizer<float> randomizer(-maxDistanceFromCenter, maxDistanceFromCenter);
	Position randomPoint(center.getX() + randomizer.generateRandomValue(), center.getY() + randomizer.generateRandomValue());
	return randomPoint;
}