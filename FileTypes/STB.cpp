#include <algorithm>
#include "STB.h"
#include "..\..\JBROSE_Common\DirectoryParser.h"
#include "..\..\JBROSE_Common\FileReader.h"
#include "..\..\JBROSE_Common\FileStoredReader.h"
#include "..\..\JBROSE_Common\Logger.h"

ROSEThreadedLogger logger;

STBFile::STBFile(const char *filePath) {
	this->filePath = std::string(filePath);
	logger.logDebug("Loading STB: ", this->filePath.substr(this->filePath.find_last_of("\\") + 1).c_str());
	if (USE_TRANSLATIONS) {
		logger.logDebug("Applying translation file to STB-entries.");
		std::string stlPath = this->filePath.substr(0, this->filePath.find_last_of("."));
		stlPath += "_S.STL";
		if (DirectoryParser::isFileExistent(stlPath.c_str())) {
			logger.logDebug("Loading Translations-File: ", stlPath.substr(stlPath.find_last_of("\\") + 1).c_str());
			FileStoredReader reader(stlPath.c_str());
			translations = new STLFile(reader);
		}
	}
	logger.logDebug("Starting to read STB content...");
	readContent();
	logger.logDebug("Finished loading STB: ", this->filePath.substr(this->filePath.find_last_of("\\") + 1).c_str());
}

STBFile::~STBFile() {
	std::for_each(entries.cbegin(), entries.cend(), [](std::pair<uint32_t, STBEntry*> pair) {
		delete pair.second;
		pair.second = nullptr;
	});
}

void STBFile::readContent() {
	FileStoredReader reader(filePath.c_str());
	if (reader.isValid()) {
		reader.skipBytes(sizeof(uint32_t));
		uint32_t offset = reader.readUInt();
		rows = static_cast<uint16_t>(reader.readUInt() - 1);
		logger.logTrace("Found a total of ", rows, " rows.");
		columns = reader.readUInt() - 1;
		logger.logTrace("Found a total of ", columns, " columns.");
		entries.reserve(rows);
		reader.resetCaretTo(offset);
		
		for (uint16_t i = 0; i < rows; i++) {
			auto entry = new STBEntry(reader, i, columns);
			if (USE_TRANSLATIONS) {
				entry->updateTranslations(translations);
			}
			entries.insert(std::move(std::make_pair(i, entry)));
		}
	}
}

STBEntry::STBEntry(FileReader& reader, uint32_t rowId, uint16_t columnAmount) {
	this->rowId = rowId;
	this->columnAmount = columnAmount;
	values.reserve(columnAmount);
	valuesAsInt.reserve(columnAmount);
	for (uint16_t i = 0; i < columnAmount; i++) {
		uint16_t length = reader.readUShort();
		auto stringData = reader.readString(length);
		values.insert(std::move(std::make_pair(i, stringData)));
		valuesAsInt.insert(std::move(std::make_pair(i, static_cast<uint32_t>(atol(stringData)))));
	}
}

STBEntry::~STBEntry() {
	for (auto valuePair : values) {
		delete valuePair.second;
	}
	values.clear();
}

void STBEntry::updateTranslations(STLFile *file) {
	if (file == nullptr) {
		return;
	}
	auto entry = file->getEntry(rowId);
	if (entry) {
		auto shortDescription = entry->getShortDescription();

		const char* nonTranslatedDescription = values.at(0);
		delete[] nonTranslatedDescription;

		values.erase(values.begin());
		values.insert(values.cbegin(), std::make_pair(0, shortDescription));
	}
}