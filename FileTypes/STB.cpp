#include <algorithm>
#include "STB.h"
#include "..\..\JBROSE_Common\DirectoryParser.h"

STBFile::STBFile(const char *filePath) {
	this->filePath = std::string(filePath);
	std::cout << "Loading STB: " << this->filePath.substr(this->filePath.find_last_of("\\")+1).c_str() << "\n";
	if (USE_TRANSLATIONS) {
		std::string stlPath = this->filePath.substr(0, this->filePath.find_last_of("."));
		stlPath += "_S.STL";
		if (DirectoryParser::isFileExistent(stlPath.c_str())) {
			std::cout << "Loading Translations-File: " << stlPath.substr(stlPath.find_last_of("\\") + 1).c_str() << "\n";
			translations = new STLFile(FileReader(stlPath.c_str()));
		}
	}
	readContent();
}

STBFile::~STBFile() {
	std::for_each(entries.cbegin(), entries.cend(), [](std::pair<uint32_t, STBEntry*> pair) {
		delete pair.second;
		pair.second = nullptr;
	});
}

void STBFile::readContent() {
	FileReader reader(filePath.c_str());
	reader.skipBytes(4);
	uint32_t offset = reader.readUInt();
	rows = static_cast<uint16_t>(reader.readUInt() - 1);
	columns = reader.readUInt() - 1;
	entries.reserve(rows);
	reader.setCaret(offset);
	for (uint16_t i = 0; i < rows; i++) {
		auto entry = new STBEntry(reader, i, columns);
		if (USE_TRANSLATIONS) {
			entry->updateTranslations(translations);
		}
		entries.insert(std::make_pair(i, entry));
	}
}

STBEntry::STBEntry(FileReader& reader, uint32_t rowId, uint16_t columnAmount) {
	this->rowId = rowId;
	this->columnAmount = columnAmount;
	values.reserve(columnAmount);
	for (uint16_t i = 0; i < columnAmount; i++) {
		uint16_t length = reader.readUShort();
		auto stringData = reader.readString(length);
		values.insert(std::make_pair(i, stringData));
		valuesAsInt.insert(std::make_pair(i, static_cast<uint32_t>(atol(stringData.get()))));
	}

}

STBEntry::~STBEntry() {

}

void STBEntry::updateTranslations(STLFile *file) {
	if (file == nullptr) {
		return;
	}
	auto entry = file->getEntry(rowId);
	if (entry) {
		auto shortDescription = entry->getShortDescription();
		values.at(0) = shortDescription;
	}
}