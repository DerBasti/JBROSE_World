#ifndef __ROSE_POSITION__
#define __ROSE_POSITION__
#pragma once
#include <mutex>
#include <memory>
#include <inttypes.h>
#include <thread>
#include "..\..\JBROSE_Common\Timer.h"
#include "../../JBROSE_Common/Logger.h"

class Position {
private:
	float x;
	float y;

public:
	Position();
	Position(const Position& pos);
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

	__inline float toLength() const {
		return ::sqrtf((getX() * getX()) + (getY() * getY()));
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

std::ostream& operator<<(std::ostream& out, const Position& pos);
std::wostream& operator<<(std::wostream& out, const Position& pos);

class MapPosition {
public:
	using UpdateDestinationVisuallyCallback = bool(*)();
private:
	Position current;
	std::mutex currentPositionMutex;
	Position destination;
	std::mutex destinationPositionMutex;
	Position source;
	std::mutex sourcePositionMutex;
	float direction;

	std::function<bool()> updateCallback;
public:
	MapPosition();
	MapPosition(const Position& current);
	MapPosition(const Position& current, const Position& destination);
	MapPosition(const MapPosition& collection);
	virtual ~MapPosition();

	__inline Position getCurrentPosition() const {
		return Position(current);
	}
	
	__inline void setCurrentPosition(Position current) {
		std::lock_guard<std::mutex> lock(currentPositionMutex);
		this->current = std::move(current);
	}

	__inline Position getDestinationPosition() const {
		return Position(destination);
	}

	__inline void setDestinationPosition(Position destination) {
		std::lock_guard<std::mutex> lock(destinationPositionMutex);
		this->destination = std::move(destination);
	}
	__inline void setDestinationPositionToCurrentPosition() {
		setDestinationPosition(getCurrentPosition());
	}

	__inline bool setDestinationPositionVisually(Position destination) {
		setDestinationPosition(std::move(destination));
		return updateCallback();
	}

	__inline Position getSourcePosition() const {
		return Position(source);
	}

	__inline void setSourcePosition(Position source) {
		std::lock_guard<std::mutex> lock(destinationPositionMutex);
		this->source = std::move(source);
	}

	__inline void setDirection(const float dir) {
		direction = dir;
	}
	__inline float getDirection() const {
		return direction;
	}

	__inline void setUpdateDestinationVisualCallback(std::function<bool()> callback) {
		updateCallback = callback;
	}
};

#endif //__ROSE_POSITION__