#include "Position.h"
#include <iostream>

Position::Position() : Position(0.0f, 0.0f) {

}

Position::Position(const Position& other) : Position(other.getX(), other.getY()) {

}

Position::Position(float _x, float _y) {
	x = _x;
	y = _y;
}

Position::~Position() {

}


std::ostream& operator<<(std::ostream& out, const Position& pos) {
	out << "[X: " << pos.getX() << ", Y: " << pos.getY() << "]";
	return out;
}

std::wostream& operator<<(std::wostream& out, const Position& pos) {
	out << "[X: " << pos.getX() << ", Y: " << pos.getY() << "]";
	return out;
}


MapPosition::MapPosition() : MapPosition(Position()) {

}

MapPosition::MapPosition(const Position& current) : MapPosition(current, current) {

}

MapPosition::MapPosition(const Position& current, const Position& destination) {
	this->current = current;
	this->destination = destination;
	this->source = current;
	direction = 0.0f;
}

MapPosition::MapPosition(const MapPosition& collection) {
	current = collection.getCurrentPosition();
	destination = collection.getDestinationPosition();
	source = collection.getSourcePosition();
	direction = collection.getDirection();
}

MapPosition::~MapPosition() {

}