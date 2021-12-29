#ifndef __ROSE_IFO_FILE__
#define __ROSE_IFO_FILE__

#pragma once

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <type_traits>

#include "..\..\JBROSE_Common\FileReader.h"
#include "..\BasicTypes\Position.h"

class IFOEntry {
private:
	std::shared_ptr<char> stringData;
	uint16_t warpSTBId;
	uint16_t eventId;
	uint32_t objectType;
	uint32_t objectId;
	uint32_t mapSectorX;
	uint32_t mapSectorY;
	float direction;
	Position position;
	float z;
	float scaling[3];

	void readBasicStructure(FileReader& reader);
protected:
	virtual void readSpecialized(FileReader& reader);
public:
	IFOEntry(FileReader& reader);
	virtual ~IFOEntry();
	__inline uint16_t getWarpSTBId() const {
		return warpSTBId;
	}
	__inline uint32_t getObjectId() const {
		return objectId;
	}

	__inline float getDirection() const {
		return direction;
	}

	__inline const Position& getPosition() const {
		return position;
	}
	virtual std::string toPrintable() const {
		return std::string("IFOEntry");
	}
};

class IFOMonsterSpawnEntry : public IFOEntry {
public:
	class Round {
	private:
		uint32_t monsterId;
		uint32_t amount;
		bool tacticalFlag;
	public:
		Round(uint32_t monsterId, uint32_t spawnAmount, bool tacticalFlag) {
			this->monsterId = monsterId;
			amount = spawnAmount;
			this->tacticalFlag = tacticalFlag;
		}
		virtual ~Round() {}

		__inline uint32_t getMonsterId() const {
			return monsterId;
		}
		__inline uint32_t getMonsterAmount() const {
			return amount;
		}
		__inline bool isTacticalSpawn() const {
			return tacticalFlag;
		}
	};
private:
	std::shared_ptr<char> spawnName;
	std::vector<std::shared_ptr<Round>> basicRounds;
	std::vector<std::shared_ptr<Round>> tacticalRounds;
	uint8_t averageMonstersSpawnedPerBasicRound;
	uint64_t respawnInterval;
	uint32_t maximumAmountOfMonsters;
	float maximumRadiusForSpawn;
	uint32_t tacticalPointsNecessary;
	void readRounds(FileReader& reader, std::vector<std::shared_ptr<Round>>& roundVector, bool isTactical);
protected:
	virtual void readSpecialized(FileReader& reader);
public:
	IFOMonsterSpawnEntry(FileReader& reader);
	virtual ~IFOMonsterSpawnEntry();

	__inline const std::vector<std::shared_ptr<Round>>& getBasicRounds() const {
		return basicRounds;
	}
	__inline const std::vector<std::shared_ptr<Round>>& getTacticalRounds() const {
		return tacticalRounds;
	}
	__inline bool hasTacticalRounds() const {
		return !tacticalRounds.empty();
	}

	__inline uint16_t getAverageMonsterAmountPerSpawn() const {
		return averageMonstersSpawnedPerBasicRound;
	}

	__inline uint64_t getRespawnInterval() const {
		return respawnInterval;
	}
	__inline uint32_t getMaximumAmountOfMonsters() const {
		return maximumAmountOfMonsters;
	}
	__inline float getMaximumRadius() const {
		return maximumRadiusForSpawn;
	}
	__inline uint32_t getTacticalPointsNecessary() const {
		return tacticalPointsNecessary;
	}
	virtual std::string toPrintable() const;

};

class IFONPCLocationEntry : public IFOEntry {
private:
	uint32_t aiId;
	std::shared_ptr<char> conFile;
protected:
	virtual void readSpecialized(FileReader& reader);
public:
	IFONPCLocationEntry(FileReader& reader);
	virtual ~IFONPCLocationEntry();
	__inline uint32_t getAiId() const {
		return aiId;
	}
	__inline std::shared_ptr<char> getCONFileName() const {
		return conFile;
	}
};

class IFOFile {
public:
	enum class BlockType : uint32_t {
		ECONOMY, DECORATION, NPCLOCATION, BUILDING, SOUND, EFFECTS, ANIMATION, WATER,
		MONSTERSPAWN, WATERPLANE, TELEGATE, COLLISION, TRIGGER
	};
private:
	uint32_t blockCount;
	std::unordered_map<BlockType, std::vector<std::shared_ptr<IFOEntry>>> entries;

	template<class _T, class = typename std::enable_if<std::is_base_of<IFOEntry, _T>::value>::type>
	std::vector<std::shared_ptr<IFOEntry>> readEntries(FileReader& reader, uint32_t typeAmount) {
		std::vector<std::shared_ptr<IFOEntry>> result;
		result.reserve(typeAmount);
		for (uint32_t i = 0; i < typeAmount; i++) {
			result.push_back(std::shared_ptr<_T>(new _T(reader)));
		}
		return result;
	}

	void readContent(FileReader& reader);
	std::vector<std::shared_ptr<IFOEntry>> readTypedEntry(FileReader& reader, BlockType type);
public:
	IFOFile(const char* path);
	IFOFile(const std::string& path);
	virtual ~IFOFile();

	__inline bool isTypedEntryExistent(const BlockType& type) const {
		return entries.find(type) != entries.cend();
	}

	const std::vector<std::shared_ptr<IFOEntry>>& getTypedEntry(const BlockType& type) const {
		return entries.at(type);
	}
};

#endif