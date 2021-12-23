#include "NewDestinationRequestPacket.h"
#include "../../../JBROSE_Common/FileReader.h"

NewDestinationRequestPacket::NewDestinationRequestPacket(const Packet* packet) : Packet(ID, packet->getLength()) {
	LoadedDataReader reader(packet->getRawData());
	targetLocalId = reader.readUShort();

	destinationX = reader.readFloat();
	destinationY = reader.readFloat();
}

NewDestinationRequestPacket::~NewDestinationRequestPacket() {

}

std::string NewDestinationRequestPacket::toPrintable() const {
	char buf[0x80] = { 0x00 };
	sprintf_s(buf, "[NewDestinationRequestPacket]\n\t* New destination (%.2f, %.2f) for local id: %i", destinationX, destinationY, targetLocalId);
	return std::string(buf);
}