#ifndef __ROSE_POSITION__
#define __ROSE_POSITION__
#pragma once
#include <mutex>
#include <memory>
#include <chrono>
#include <inttypes.h>
#include <thread>

class Position {
private:
	float x;
	float y;
public:
	Position();
	Position(float _x, float _y);
	virtual ~Position();

	__inline Position operator-(const Position& other) const {
		return Position(getX() - other.getX(), getY() - other.getY());
	}
	__inline Position operator+(const Position& other) const {
		return Position(getX() + other.getX(), getY() + other.getY());
	}

	__inline bool operator==(const Position& other) const {
		return getX() == other.getX() && getY() == other.getY();
	}
	__inline bool operator!=(const Position& other) const {
		return !(operator==(other));
	}

	__inline float getX() const {
		return x;
	}
	__inline void setX(const float _x) {
		x = _x;
	}
	__inline float getY() const {
		return y;
	}
	__inline void setY(const float _y) {
		y = _y;
	}
};

class PositionCollection {
private:
	Position current;
	Position destination;
	float direction;
public:
	PositionCollection();
	PositionCollection(const Position& current);
	PositionCollection(const Position& current, const Position& destination);
	PositionCollection(const PositionCollection& collection);
	virtual ~PositionCollection();

	__inline Position getCurrentPosition() const {
		return current;
	}
	__inline void setCurrentPosition(const Position& current) {
		this->current = current;
	}
	__inline Position getDestinationPosition() const {
		return destination;
	}
	__inline void setDestinationPosition(const Position& destination) {
		this->destination = destination;
	}
	__inline void setDirection(const float dir) {
		direction = dir;
	}
	__inline float getDirection() const {
		return direction;
	}
};

class Timer {
private:
	std::chrono::system_clock clock;
	uint64_t timestamp;

	__inline uint64_t getCurrentTimeInMillis() const {
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now().time_since_epoch());
		return ms.count();
	}
public:
	Timer() { 
		timestamp = getCurrentTimeInMillis();
	}
	virtual ~Timer() { }

	__inline uint64_t getPassedTimeInMillis() const {
		return getCurrentTimeInMillis() - timestamp;
	}
	__inline uint64_t updateTimestamp() {
		uint64_t result = getPassedTimeInMillis();
		timestamp = getCurrentTimeInMillis();
		return result;
	}
};

class PositionProcessor {
private:
	std::shared_ptr<PositionCollection> position;
	Timer timer;
	uint64_t lastTimestampSinceMove;

	__inline float getDistanceX() const {
		return position->getDestinationPosition().getX() - position->getCurrentPosition().getX();
	}
	__inline float getDistanceY() const {
		return position->getDestinationPosition().getY() - position->getCurrentPosition().getY();
	}
public:
	PositionProcessor(std::shared_ptr<PositionCollection> pos);
	virtual ~PositionProcessor();

	bool processNewPosition();
	__inline float getDistanceToDestination() const {
		return getDistanceBetweenPoints(position->getCurrentPosition(), position->getDestinationPosition());
	}
	static float getDistanceBetweenPoints(const Position& first, const Position& second);
};

#endif //__ROSE_POSITION__