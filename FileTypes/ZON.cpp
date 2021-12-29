#include "ZON.h"

ZONFile::ZONFile(const char* path) {
	filePath = std::string(path);
	FileInputReader reader(path);
	std::vector<ZoneData::ZoneType> zoneTypes;
	std::vector<uint32_t> offsetsInFile;

	zoneTypeAmount = reader.readUInt();
	for (uint32_t i = 0; i < zoneTypeAmount; i++) {
		zoneTypes.emplace_back(std::move(ZoneData::ZoneType(reader.readUInt())));
		offsetsInFile.emplace_back(reader.readUInt());
	}

	for (uint32_t i = 0; i < zoneTypeAmount; i++) {
		reader.resetCaretTo(offsetsInFile.at(i));
		switch (zoneTypes.at(i)) {
			case ZoneData::ZoneType::GENERIC:
				genericData.push_back(new GenericZoneData(reader));
			break;
			case ZoneData::ZoneType::EVENT:
			{
				uint32_t entryAmount = reader.readUInt();
				for (uint32_t j = 0; j < entryAmount; j++) {
					eventData.push_back(new EventZoneData(reader, j));
				}
			}
			break;
			case ZoneData::ZoneType::ECONOMY:
				economyData.push_back(new EconomyZoneData(reader));
			break;
		}
	}
	auto event = getEventByName("start");
	if (event != nullptr) {
		mapCenter = event->getCenterPosition();
	}
}


std::vector<EventZoneData*> ZONFile::getAllEventWithName(const char* name) const {
	std::vector<EventZoneData*> events;
	for (auto event : eventData) {
		if (_stricmp(event->getEventName().get(), name) == 0) {
			events.push_back(event);
		}
	}
	return events;
}

ZONFile::~ZONFile() {
	std::for_each(economyData.begin(), economyData.end(), [](EconomyZoneData* data) {
		delete data;
		data = nullptr;
	});
	std::for_each(eventData.begin(), eventData.end(), [](EventZoneData* data) {
		delete data;
		data = nullptr;
	});
	std::for_each(genericData.begin(), genericData.end(), [](GenericZoneData* data) {
		delete data;
		data = nullptr;
	});
}

EventZoneData* ZONFile::getEventByName(const char* name) const {
	size_t i = 0;
	size_t amount = eventData.size();
	for (auto eventPtr = eventData.data(); i<amount; i++, eventPtr++) {
		auto event = *eventPtr;
		if (_stricmp(event->getEventName().get(), name) == 0) {
			return event;
		}
	}
	return nullptr;
}

EventZoneData* ZONFile::getEventByName(const char* name, const char* defaultEventName) const {
	EventZoneData* zoneDataToBeFound = getEventByName(name);
	if (!zoneDataToBeFound) {
		zoneDataToBeFound = getEventByName(defaultEventName);
	}
	return zoneDataToBeFound;
}

std::vector<EventZoneData*> ZONFile::getAllRestorePoints() const {
	std::vector<EventZoneData*> restorePoints = std::move(getAllEventWithName("restore"));
	restorePoints.push_back(getEventByName("start"));
	return restorePoints;
}

ZoneData::ZoneData(const ZoneType& type) {
	this->type = type;
}

ZoneData::~ZoneData() {

}

EconomyZoneData::EconomyZoneData(FileReader& reader) : ZoneData(ZoneType::ECONOMY) {
	readEconomyData(reader);
}

EconomyZoneData::~EconomyZoneData() {

}

void EconomyZoneData::readEconomyData(FileReader& reader) {
	uint8_t stringLength = reader.readByte();
	unknownString = reader.readStringWrapped(stringLength);

	dungeonFlag = reader.readUInt() > 0;

	stringLength = reader.readByte();
	musicName = reader.readStringWrapped(stringLength);

	stringLength = reader.readByte();
	modelName = reader.readStringWrapped(stringLength);

	townCounter = reader.readUInt();
	populationCounter = reader.readUInt();
	developmentCounter = reader.readUInt();
	for (uint8_t i = 0; i < 10; i++) {
		consumerate[i] = reader.readUInt();
	}
}


EventZoneData::EventZoneData(FileReader& reader, const uint8_t eventId) : ZoneData(ZoneType::EVENT) {
		readEvent(reader);
		this->eventId = eventId;
}

EventZoneData::~EventZoneData() {

}

void EventZoneData::readEvent(FileReader& reader) {
	float x = reader.readFloat() + 520000.0f;
	z = reader.readFloat();
	float y = reader.readFloat() + 520000.0f;

	center = Position(x, y);

	uint8_t eventNameLength = reader.readByte();
	if (eventNameLength > 0) {
		eventName = reader.readStringWrapped(eventNameLength);
	}
}

GenericZoneData::GenericZoneData(FileReader& reader) : ZoneData(ZoneType::GENERIC) {
	readGenericData(reader);
}

GenericZoneData::~GenericZoneData() {

}

void GenericZoneData::readGenericData(FileReader& reader) {
	unknown = reader.readUInt();
	mapHeight = reader.readUInt();
	mapWidth = reader.readUInt();
	patchGridSize = reader.readUInt();

	gridSize = reader.readFloat();
	patchSize = reader.readFloat();

	centerIfoX = static_cast<uint8_t>(reader.readUInt());
	centerIfoY = static_cast<uint8_t>(reader.readUInt());
}


RestorePoint::RestorePoint(uint32_t id, uint16_t mapId, EventZoneData* rawRestorePointData) {
	this->restorePointId = id;
	this->mapId = mapId;
	this->center = rawRestorePointData->getCenterPosition();
	this->restorePointName = rawRestorePointData->getEventName();
}

RestorePoint::~RestorePoint() {

}

const char* RestorePoint::DEFAULT_RESTORE_POINT_NAME = "restore";