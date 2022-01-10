#include "UseConsumableRequestPacket.h"


UseConsumableRequestPacket::UseConsumableRequestPacket(const Packet* packet) : Packet(packet->getCommandId(), packet->getLength()) {
	LoadedDataReader interpreter(packet->getRawData());
	slotId = interpreter.readByte();
	if(packet->getLength() > 7) {
		repairSlotId = interpreter.readByte();
	}
}

UseConsumableRequestPacket::~UseConsumableRequestPacket() {

}