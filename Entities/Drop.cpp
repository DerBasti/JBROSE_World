#include "Drop.h"
#include "../Map/Map.h"
#include "../WorldClient.h"

Drop::Drop(const Item& item, const Position& pos) {
	visualityProcessor = new DropVisualityProcessor(this);
	this->item = item;

	getLocationData()->getMapPosition()->setDestinationPosition(pos);
	getLocationData()->getMapPosition()->setCurrentPosition(pos);
	getCombat()->setTeamId(EntityDefaultTeamId::DROP);

#ifdef _DEBUG
	name = std::shared_ptr<char>(new char[0x64], std::default_delete<char[]>());
	if (item.getType() == ItemTypeList::MONEY) {
		sprintf_s(name.get(), 0x64, "%i Zuly", item.getAmount());
	}
	else {
		sprintf_s(name.get(), 0x64, "'%s'-Drop of ID %i with amount %i", item.getType().getTypeName(), item.getId(), item.getAmount());
	}
#endif
}

Drop::~Drop() {

}

const char* Drop::getName() const {
#ifdef _DEBUG
	return name.get();
#else
	return "Drop";
#endif
}