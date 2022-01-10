#ifndef __ROSE_STB__
#define __ROSE_STB__
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "STL.h"

class STBEntry {
private:
	uint32_t rowId;
	uint16_t columnAmount;
	std::unordered_map <uint16_t, std::shared_ptr<char>> values;
	std::unordered_map <uint16_t, uint32_t> valuesAsInt;
public:
	STBEntry(class FileReader& reader, uint32_t rowId, uint16_t columnAmount);
	virtual ~STBEntry();

	void updateTranslations(STLFile* translations);

	__inline const char* getColumnData(const uint16_t column) const {
		return values.at(column).get();
	}
	__inline uint32_t getColumnDataAsInt(const uint16_t column) const {
		return valuesAsInt.at(column);
	}
	__inline uint16_t getColumnAmount() const {
		return columnAmount;
	}
};

class STBFile {
private:
	STLFile* translations;
	std::string filePath;
	uint16_t columns;
	uint32_t rows;
	std::unordered_map<uint16_t, STBEntry*> entries;

	void readContent();
public:
	const static bool USE_TRANSLATIONS = true;
	STBFile(const char *filePath);
	virtual ~STBFile();

	__inline const std::unordered_map<uint16_t, STBEntry*>& getAllEntries() const {
		return entries;
	}

	__inline const STBEntry* getEntry(const uint32_t row) const {
		return entries.at(row);
	}
	__inline uint32_t getEntryAmount() const {
		return rows;
	}
	__inline uint16_t getColumnsPerEntry() const {
		return columns;
	}
};

class EquipmentSTB : public STBFile {
private:
	__inline uint16_t getValueOfEntry(uint16_t id, uint16_t type) const {
		return static_cast<uint16_t>(getEntry(id)->getColumnDataAsInt(type));
	}
public:
	const static uint16_t TYPE = 0x04;
	const static uint16_t PRICE = 0x05;
	const static uint16_t PRICE_RATE = 0x06;
	const static uint16_t WEIGHT = 0x07;
	const static uint16_t QUALITY = 0x08;
	const static uint16_t REQUIRED_LEVEL = 0x0D;
	const static uint16_t CRAFTING_STB_REFERENCE = 0x0E;
	const static uint16_t UNION_POINTS = 0x0F;
	const static uint16_t STAT_FIRST_TYPE = 0x18;
	const static uint16_t STAT_FIRST_AMOUNT = 0x19;
	const static uint16_t STAT_SECOND_TYPE = 0x1A;
	const static uint16_t STAT_SECOND_AMOUNT = 0x1B;
	const static uint16_t DURABILITY_MINIMUM = 0x1D;
	const static uint16_t DEFENSE_PHYISCAL = 0x1F;
	const static uint16_t DEFENSE_MAGICAL = 0x20;
	const static uint16_t ATTACK_RANGE = 0x21;
	const static uint16_t MOVEMENT_SPEED = 0x21;
	const static uint16_t MOTION_COLUMN = 0x22;
	const static uint16_t ATTACK_POWER_PHYSICAL = 0x23;
	const static uint16_t ATTACK_SPEED = 0x24;
	const static uint16_t ATTACK_POWER_MAGICAL = 0x25;

	EquipmentSTB(const char *filePath) : STBFile(filePath) {}
	virtual ~EquipmentSTB() {}

	__inline uint16_t getAttackPowerOfEntry(uint16_t id) const {
		return getValueOfEntry(id, ATTACK_POWER_PHYSICAL);
	}
	__inline uint16_t getMagicAttackPowerOfEntry(uint16_t id) const {
		return getValueOfEntry(id, ATTACK_POWER_MAGICAL);
	}
	__inline uint16_t getAttackSpeedOfEntry(uint16_t id) const {
		return getValueOfEntry(id, ATTACK_SPEED);
	}
	__inline uint16_t getAttackRangeOfEntry(uint16_t id) const {
		return getValueOfEntry(id, ATTACK_RANGE);
	}
	__inline uint16_t getDefenseOfEntry(uint16_t id) const {
		return getValueOfEntry(id, DEFENSE_PHYISCAL);
	}
	__inline uint16_t getMagicalDefenseOfEntry(uint16_t id) const {
		return getValueOfEntry(id, DEFENSE_MAGICAL);
	}
	__inline uint16_t getMovementSpeedOfEntry(uint16_t id) const {
		return getValueOfEntry(id, MOVEMENT_SPEED);
	}
	__inline uint16_t getEquipmentType(uint16_t id) const {
		return getValueOfEntry(id, TYPE);
	}
	__inline uint32_t getMotionIdOfEntry(uint16_t id) const {
		return getValueOfEntry(id, MOTION_COLUMN);
	}
	__inline uint16_t getQualityOfEntry(uint16_t id) const {
		return getValueOfEntry(id, QUALITY);
	}
	__inline uint16_t getWeightOfEntry(uint16_t id) const {
		return getValueOfEntry(id, WEIGHT);
	}
};

class StatusSTBFile : public STBFile {
private:
	__inline uint16_t getValueOfEntry(uint16_t id, uint16_t type) const {
		return static_cast<uint16_t>(getEntry(id)->getColumnDataAsInt(type));
	}
public:
	const static uint16_t STACKABLE_BUFF_FLAG_COLUMN = 0x02;
	const static uint16_t BUFF_OR_DEBUFF_FLAG_COLUMN = 0x03;
	const static uint16_t INCREASE_FIRST_VALUE_AMOUNT_COLUMN = 0x06;
	const static uint16_t INCREASE_SECOND_VALUE_AMOUNT_COLUMN = 0x08;
	StatusSTBFile(const char *filePath) : STBFile(filePath) {}
	virtual ~StatusSTBFile() {}

	__inline bool isStatusStackableOfEntry(const uint16_t id) const {
		return getValueOfEntry(id, STACKABLE_BUFF_FLAG_COLUMN) > 0;
	}
	__inline bool getBuffOrDebuffFlag(const uint16_t id) const {
		return getValueOfEntry(id, STACKABLE_BUFF_FLAG_COLUMN) > 0;
	}
	__inline uint16_t getIncreaseOfFirstValueOfEntry(const uint16_t id) const {
		return getValueOfEntry(id, INCREASE_FIRST_VALUE_AMOUNT_COLUMN);
	}
	__inline uint16_t getIncreaseOfSecondValueOfEntry(const uint16_t id) const {
		return getValueOfEntry(id, INCREASE_SECOND_VALUE_AMOUNT_COLUMN);
	}
};

class ZoneSTBFile : public STBFile {
private:
	__inline uint16_t getValueOfEntry(uint16_t id, uint16_t type) const {
		return static_cast<uint16_t>(getEntry(id)->getColumnDataAsInt(type));
	}
public:
	const static uint16_t TOTAL_DAY_TIME_COLUMN = 13;
	const static uint16_t MORNING_TIME_COLUMN = 14;
	const static uint16_t NOON_TIME_COLUMN = 15;
	const static uint16_t EVENING_COLUMN = 16;
	const static uint16_t NIGHT_COLUMN = 17;

	ZoneSTBFile(const char *filePath) : STBFile(filePath) {}
	virtual ~ZoneSTBFile() {}

	__inline uint16_t getTotalDayTimeInSeconds(uint16_t id) const {
		return getValueOfEntry(id, TOTAL_DAY_TIME_COLUMN);
	}
	__inline uint16_t getMorningStartTimeInSeconds(uint16_t id) const {
		return getValueOfEntry(id, MORNING_TIME_COLUMN);
	}
	__inline uint16_t getNoonStartTimeInSeconds(uint16_t id) const {
		return getValueOfEntry(id, NOON_TIME_COLUMN);
	}
	__inline uint16_t getEveningStartTimeInSeconds(uint16_t id) const {
		return getValueOfEntry(id, EVENING_COLUMN);
	}
	__inline uint16_t getNightStartTimeInSeconds(uint16_t id) const {
		return getValueOfEntry(id, NIGHT_COLUMN);
	}
};

class ConsumeSTBFile : public EquipmentSTB {
private:
	__inline uint16_t getValueOfEntry(uint16_t id, uint16_t type) const {
		return static_cast<uint16_t>(getEntry(id)->getColumnDataAsInt(type));
	}
public:
	const static uint16_t PRICE = 0x05;
	const static uint16_t PRICE_RATE = 0x06;
	const static uint16_t WEIGHT = 0x07;
	const static uint16_t QUALITY = 0x08;
	const static uint16_t SCRIPT_EXECUTION_TYPE_COLUMN = 0x07;
	const static uint16_t STAT_TYPE_REQUIRED_COLUMN = 0x11;
	const static uint16_t STAT_AMOUNT_REQUIRED_COLUMN = 0x12;

	const static uint16_t STAT_TYPE_TO_ADD_TO_COLUMN = 0x13;
	const static uint16_t STAT_AMOUNT_TO_ADD_TO_COLUMN = 0x14;
	const static uint16_t STATUS_STB_REFERENCE_COLUMN = 0x18;

	ConsumeSTBFile(const char *filePath) : EquipmentSTB(filePath) {}
	virtual ~ConsumeSTBFile() {}

	__inline uint16_t getWeightOfEntry(uint16_t id) const {
		return getValueOfEntry(id, WEIGHT);
	}
	__inline uint16_t getQualityOfEntry(uint16_t id) const {
		return getValueOfEntry(id, QUALITY);
	}
	__inline uint16_t getScriptExecutionTypeOfEntry(uint16_t id) const {
		return getValueOfEntry(id, SCRIPT_EXECUTION_TYPE_COLUMN);
	}
	__inline uint16_t getRequiredStatTypeOfEntry(uint16_t id) const {
		return getValueOfEntry(id, STAT_TYPE_REQUIRED_COLUMN);
	}
	__inline uint16_t getRequiredStatAmountOfEntry(uint16_t id) const {
		return getValueOfEntry(id, STAT_AMOUNT_REQUIRED_COLUMN);
	}
	__inline uint16_t getStatTypeToAddOfEntry(uint16_t id) const {
		return getValueOfEntry(id, STAT_TYPE_TO_ADD_TO_COLUMN);
	}
	__inline uint16_t getStatAmountToAddOfEntry(uint16_t id) const {
		return getValueOfEntry(id, STAT_AMOUNT_TO_ADD_TO_COLUMN);
	}
	__inline uint16_t getStatusStbReferenceOfEntry(uint16_t id) const {
		return getValueOfEntry(id, STATUS_STB_REFERENCE_COLUMN);
	}
};

#endif //__ROSE_STB__