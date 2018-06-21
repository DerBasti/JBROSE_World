#include <algorithm>
#include "STB.h"

STBFile::STBFile(const char *filePath) {
	this->filePath = std::string(filePath);
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
		auto entry = new STBEntry(reader, columns);
		entries.insert(std::make_pair(i, entry));
	}
}

STBEntry::STBEntry(FileReader& reader, uint16_t columnAmount) {
	this->columnAmount = columnAmount,
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