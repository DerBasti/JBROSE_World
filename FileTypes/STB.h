#ifndef __ROSE_STB__
#define __ROSE_STB__
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "..\..\JBROSE_Common\FileReader.h"

class STBEntry {
private:
	uint16_t columnAmount;
	std::unordered_map <uint16_t, std::shared_ptr<char>> values;
	std::unordered_map <uint16_t, uint32_t> valuesAsInt;
public:
	STBEntry(FileReader& reader, uint16_t columnAmount);
	virtual ~STBEntry();

	__inline std::shared_ptr<char> getColumnData(const uint16_t column) const {
		return values.at(column);
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
	std::string filePath;
	uint16_t columns;
	uint32_t rows;
	std::unordered_map<uint16_t, STBEntry*> entries;

	void readContent();
public:
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

#endif //__ROSE_STB__