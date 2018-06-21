#include "Position.h"
#include <iostream>

Position::Position() : Position(0.0f, 0.0f) {

}

Position::Position(float _x, float _y) {
	x = _x;
	y = _y;
}

Position::~Position() {

}


PositionCollection::PositionCollection() : PositionCollection(Position()) {

}

PositionCollection::PositionCollection(const Position& current) : PositionCollection(current, current) {

}

PositionCollection::PositionCollection(const Position& current, const Position& destination) {
	this->current = current;
	this->destination = destination;
	direction = 0.0f;
}

PositionCollection::PositionCollection(const PositionCollection& collection) {
	current = collection.getCurrentPosition();
	destination = collection.getDestinationPosition();
	direction = collection.getDirection();
}

PositionCollection::~PositionCollection() {

}


PositionProcessor::PositionProcessor(std::shared_ptr<PositionCollection> pos) : position(pos) {

}

PositionProcessor::~PositionProcessor() {

}

bool PositionProcessor::processNewPosition() {
	float timePassed = static_cast<float>(timer.updateTimestamp());
	if (position->getCurrentPosition() == position->getDestinationPosition()) {
		return false;
	}
	const float movementSpeed = 425.0f;
	float distanceMoved = (timePassed / 1000.0f) * movementSpeed; //MovementSpeed
	float totalDistance = getDistanceToDestination();
	Position newCurrent;
	if (distanceMoved > totalDistance) {
		newCurrent = position->getDestinationPosition();
		std::cout << "Destination reached at: " << newCurrent.getX() << ", " << newCurrent.getY() << "\n";
	}
	else {
		float ratios[2] = { getDistanceX() / totalDistance, getDistanceY() / totalDistance };

		newCurrent = Position(position->getCurrentPosition().getX() + (ratios[0] * distanceMoved),
			position->getCurrentPosition().getY() + (ratios[1] * distanceMoved));
	}

	position->setCurrentPosition(newCurrent);

	return true;
}

float PositionProcessor::getDistanceBetweenPoints(const Position& first, const Position& second) {
	float xDist = first.getX() - second.getX();
	float yDist = first.getY() - second.getY();

	return sqrtf((xDist*xDist) + (yDist*yDist));
}