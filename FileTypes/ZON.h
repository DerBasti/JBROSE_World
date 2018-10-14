#ifndef __ROSE_ZON_FILETYPE__
#define __ROSE_ZON_FILETYPE__
#pragma once

#include "..\..\JBROSE_Common\FileReader.h"
#include "..\BasicTypes\Position.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

//Marker interface
class ZoneData {
public:
	enum class ZoneType : uint32_t {
		GENERIC, EVENT, TEXTURE, TILELIST, ECONOMY
	};
private:
	ZoneType type;
public:
	ZoneData(const ZoneType& type);
	virtual ~ZoneData();

	__inline const ZoneType& getZoneType() const {
		return type;
	}
};

class EconomyZoneData : public ZoneData {
private:
	std::shared_ptr<char> unknownString;
	bool dungeonFlag;
	std::shared_ptr<char> musicName;
	std::shared_ptr<char> modelName;
	uint32_t townCounter;
	uint32_t populationCounter;
	uint32_t developmentCounter;
	uint32_t consumerate[10];

	void readEconomyData(FileReader& reader);
public:
	EconomyZoneData(FileReader& reader);
	virtual ~EconomyZoneData();

	__inline std::shared_ptr<char> getUnknownString() const {
		return unknownString;
	}
	__inline bool isDungeon() const {
		return dungeonFlag;
	}

	__inline std::shared_ptr<char> getMusicName() const {
		return musicName;
	}
	__inline std::shared_ptr<char> getModelName() const {
		return modelName;
	}
	__inline uint32_t getTownCounter() const {
		return townCounter;
	}
	__inline uint32_t getPopulationCounter() const {
		return populationCounter;
	}
	__inline uint32_t getDevelopmentCounter() const {
		return developmentCounter;
	}
	__inline uint32_t getConsumRate(uint8_t indexOutOfTen) const {
		return consumerate[(indexOutOfTen >= 10 ? 9 : indexOutOfTen)];
	}
};


class EventZoneData : public ZoneData {
private:
	Position center;
	float z;
	uint8_t eventId;
	std::shared_ptr<char> eventName;
	void readEvent(FileReader& reader);
public:
	constexpr static const char* DEFAULT_EVENT = "start";
	EventZoneData(FileReader& reader, const uint8_t eventId);
	virtual ~EventZoneData();
	__inline const Position& getCenterPosition() const {
		return center;
	}
	__inline uint8_t getEventId() const {
		return eventId;
	}
	__inline const std::shared_ptr<char>& getEventName() const {
		return eventName;
	}
	__inline bool isValid() const {
		return getEventId() != 0 && eventName && center.getX() != 0.0f && center.getY() != 0.0f;
	}
};

class GenericZoneData : public ZoneData {
private:
	uint32_t unknown;
	uint32_t mapHeight;
	uint32_t mapWidth;
	uint32_t patchGridSize;
	float gridSize;
	float patchSize;
	uint8_t centerIfoX;
	uint8_t centerIfoY;
	void readGenericData(FileReader& reader);
public:
	GenericZoneData(FileReader& reader);
	virtual ~GenericZoneData();
};

class ZONFile {
private:
	std::string filePath;
	uint32_t zoneTypeAmount;
	std::vector<EconomyZoneData*> economyData;
	std::vector<EventZoneData*> eventData;
	std::vector<GenericZoneData*> genericData;
	Position mapCenter;
public:
	ZONFile(const char* path);
	virtual ~ZONFile();
	EventZoneData* getEventByName(const char* name) const;
	EventZoneData* getEventByName(const char* name, const char* defaultEvent) const;

	__inline EventZoneData* getEventById(uint32_t id) const {
		return eventData.at(id);
	}
};

#endif 