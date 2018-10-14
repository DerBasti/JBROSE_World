#include "NPC.h"
#include "..\WorldPackets\Responses\SpawnMonsterVisuallyResponsePacket.h"
#include "..\FileTypes\STB.h"

NPCDefaultStatValues::NPCDefaultStatValues(uint16_t id, STBEntry* entry) {
	this->id = id;
	level = entry->getColumnDataAsInt(7);
	hpPerLevel = entry->getColumnDataAsInt(8);
	maxHp = level * hpPerLevel;
	experiencePoints = entry->getColumnDataAsInt(17);
}
NPCDefaultStatValues::~NPCDefaultStatValues() {

}

NPC::NPC(NPCDefaultStatValues* defaultValues, const Position& spawnPosition) {
	getLocationData()->getPositionCollection()->setCurrentPosition(spawnPosition);
	getLocationData()->getPositionCollection()->setDestinationPosition(spawnPosition);

	defaultStatValues = defaultValues;

	visualityProcessor = new NPCVisualityProcessor(this);
}

NPC::~NPC() {
	delete visualityProcessor;
	visualityProcessor = nullptr;
}