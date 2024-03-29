#ifndef __ROSE_STL__
#define __ROSE_STL__
#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../JBROSE_Common/Logger.h"

class STLEntryHeader {
private:
	uint32_t id;
	std::shared_ptr<char> title;
public:
	STLEntryHeader(uint32_t id, std::shared_ptr<char>& title) {
		this->id = id;
		this->title = title;
	}
	virtual ~STLEntryHeader() {

	}
	__inline uint32_t getId() const {
		return id;
	}
	__inline const std::shared_ptr<char>& getTitle() const {
		return title;
	}
};

class STLEntry {
	STLEntryHeader* header;
	const char* shortDescription;
	const char* description;
public:

	STLEntry(STLEntryHeader* header) {
		this->header = header;
	}

	__inline void addShortAndRegularDescription(const char* shortDescription, const char* description) {
		this->shortDescription = shortDescription;
		this->description = description;
	}

	__inline STLEntryHeader* getHeader() const {
		return header;
	}

	__inline const char* getShortDescription() const {
		return shortDescription;
	}
	__inline const char* getDescription() const {
		return description;
	}
};

enum STLLanguage : uint32_t {
	JAPANESE,
	ENGLISH,
	TAIWANESE,
	LANGUAGE_MAX = 5
};

class STLFile {
private:
	constexpr static const char* STL_FILETYPE_WITH_DESCRIPTION = "ITST01";
	bool descriptionsFlag;
	std::unordered_map<STLLanguage, std::unordered_map<uint32_t, STLEntry*>> entries;

	std::vector<STLEntryHeader*> readAllEntryHeader(class FileReader& reader, const uint32_t &entryAmount);
	std::vector<uint32_t> readLanguageOffsets(class FileReader& reader);
	void readAllEntryBodies(FileReader& reader, std::vector<STLEntryHeader*>& header, std::vector<uint32_t>& languageOffsets);
	const char* readDescriptionWithPossibleLengthOverflow(class FileReader& reader);
	ROSELogger logger;
public:
	STLFile(class FileReader& reader);
	virtual ~STLFile();
	
	std::unordered_map<uint32_t, STLEntry*> getEntriesByLanguage(STLLanguage language) const {
		auto entry = entries.find(language);
		if (entry != entries.cend()) {
			return entry->second;
		}
		return std::unordered_map<uint32_t, STLEntry*>();
	}

	STLEntry* getEntry(uint32_t id) {
		auto englishEntries = getEntriesByLanguage(STLLanguage::ENGLISH);
		auto iteratorPosition = englishEntries.find(id);
		return iteratorPosition != englishEntries.cend() ? iteratorPosition->second : nullptr;
	}
};


#endif //__ROSE_STL__