#include "NewDestinationRequestPacket.h"


NewDestinationRequestPacket::NewDestinationRequestPacket(const Packet* packet) : Packet(ID, packet->getLength()) {
	const char *basicPtr = packet->getRawData();
	const uint16_t *wordPtr = reinterpret_cast<const uint16_t*>(basicPtr);
	targetLocalId = *wordPtr;

	basicPtr += 2;

	const float *floatPtr = reinterpret_cast<const float*>(basicPtr);
	destinationX = *floatPtr;
	floatPtr++;

	destinationY = *floatPtr;
}

NewDestinationRequestPacket::~NewDestinationRequestPacket() {

}