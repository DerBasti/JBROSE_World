#include "STL.h"

STLFile::STLFile(FileReader& reader) {
	uint8_t fileTypeLen = reader.readByte();
	std::shared_ptr<char> fileType = reader.readString(fileTypeLen);
	descriptionsFlag = _stricmp(fileType.get(), STL_FILETYPE_WITH_DESCRIPTION) == 0;

	uint32_t entryAmount = reader.readUInt();
	std::vector<STLEntryHeader*> entryHeader = readAllEntryHeader(reader, entryAmount);
	std::vector<uint32_t> languageOffsets = readLanguageOffsets(reader);
	readAllEntryBodies(reader, entryHeader, languageOffsets);
}

STLFile::~STLFile() {

}

std::vector<STLEntryHeader*> STLFile::readAllEntryHeader(FileReader& reader, const uint32_t &entryAmount) {
	std::vector<STLEntryHeader*> entries;
	entries.reserve(entryAmount);
	for (uint32_t i = 0; i < entryAmount; i++) {
		uint8_t contentLength = reader.readByte();
		std::shared_ptr<char> content = reader.readString(contentLength);
		uint32_t id = reader.readUInt();
		entries.push_back(new STLEntryHeader(id, content));
	}
	return entries;
}

std::vector<uint32_t> STLFile::readLanguageOffsets(FileReader& reader) {
	std::vector<uint32_t> languageOffsets;
	uint32_t languageAmount = reader.readUInt();
	for (uint32_t i = 0; i < languageAmount; i++) {
		languageOffsets.push_back(reader.readUInt());
	}
	return languageOffsets;
}

void STLFile::readAllEntryBodies(FileReader& reader, std::vector<STLEntryHeader*>& header, std::vector<uint32_t>& languageOffsets) {
	uint32_t languageAmount = static_cast<uint32_t>(languageOffsets.size());
	uint32_t entryAmount = static_cast<uint32_t>(header.size());
	for (uint32_t i = 0; i < languageAmount; i++) {
		std::unordered_map<uint32_t, STLEntry*> currentLanguageEntry;
		currentLanguageEntry.reserve(entryAmount);
		reader.setCaret(languageOffsets.at(i));
		std::vector<uint32_t> stringOffsets;
		for (uint32_t j = 0; j < entryAmount; j++) {
			stringOffsets.push_back(reader.readUInt());
		}
		for (uint32_t j = 0; j < entryAmount; j++) {
			reader.setCaret(stringOffsets.at(j));
			std::shared_ptr<char> shortDescription = readDescriptionWithPossibleLengthOverflow(reader);
			std::shared_ptr<char> actualDescription;
			if (descriptionsFlag) {
				actualDescription = readDescriptionWithPossibleLengthOverflow(reader);
			}
			STLEntry* entry = new STLEntry(header.at(j));
			entry->addShortAndRegularDescription(shortDescription, actualDescription);
			currentLanguageEntry.insert(std::make_pair(entry->getHeader()->getId(), entry));
		}
		entries.insert(std::make_pair((STLLanguage)i, std::move(currentLanguageEntry)));
	}
}
std::shared_ptr<char> STLFile::readDescriptionWithPossibleLengthOverflow(FileReader& reader) {
	uint16_t descriptionLength = reader.readByte();
	if (descriptionLength >= 0x80) {
		descriptionLength = reader.readByte() << 8 | descriptionLength;
	}
	std::shared_ptr<char> description = reader.readString(descriptionLength);
	return description;
}