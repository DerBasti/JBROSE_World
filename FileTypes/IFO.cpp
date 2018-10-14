#include "IFO.h"

IFOFile::IFOFile(const char* path) {
	FileReader reader(path);
	readContent(reader);
}

IFOFile::IFOFile(const std::string& path) : IFOFile(path.c_str()) {

}

IFOFile::~IFOFile() {

}

void IFOFile::readContent(FileReader& reader) {
	blockCount = reader.readUInt();
	for (uint32_t i = 0; i < blockCount; i++) {
		BlockType type = static_cast<BlockType>(reader.readUInt());
		uint32_t typeOffset = reader.readUInt();
		if (type != BlockType::NPCLOCATION && type != BlockType::MONSTERSPAWN && type != BlockType::TELEGATE) {
			continue;
		}
		auto currentOffset = reader.getCaret();
		reader.setCaret(typeOffset);
		std::vector<std::shared_ptr<IFOEntry>> typedEntries = readTypedEntry(reader, type);
		if (!typedEntries.empty()) {
			entries.insert(std::make_pair(type, typedEntries));
		}
		reader.setCaret(currentOffset);
	}
}

std::vector<std::shared_ptr<IFOEntry>> IFOFile::readTypedEntry(FileReader& reader, BlockType type) {
	uint32_t typeAmount = reader.readUInt();
	std::vector<std::shared_ptr<IFOEntry>> result;
	switch (type) {
	case BlockType::NPCLOCATION:
		result = readEntries<IFONPCLocationEntry>(reader, typeAmount);
		break;
	case BlockType::MONSTERSPAWN:
		result = readEntries<IFOMonsterSpawnEntry>(reader, typeAmount);
		break;
	case BlockType::TELEGATE:
		result = readEntries<IFONPCLocationEntry>(reader, typeAmount);
		break;
	}
	return result;
}


IFOEntry::IFOEntry(FileReader& reader) {
	readBasicStructure(reader);
}
IFOEntry::~IFOEntry() {

}

void IFOEntry::readBasicStructure(FileReader& reader) {
	uint8_t strLen = reader.readByte();
	stringData = reader.readString(strLen);
	warpSTBId = reader.readUShort();
	eventId = reader.readUShort();
	objectType = reader.readUInt();
	objectId = reader.readUInt();
	mapSectorX = reader.readUInt();
	mapSectorY = reader.readUInt();
	float quaternion[4] = { 0.0f };
	for (uint8_t i = 0; i < 4; i++) {
		quaternion[i] = reader.readFloat();
	}

	float qw2 = pow(quaternion[0], 2);
	float qx2 = pow(quaternion[1], 2);
	float qy2 = pow(quaternion[2], 2);
	float qz2 = pow(quaternion[3], 2);
	float rad = atan2(quaternion[3] * quaternion[2], qz2 - qy2);
	direction = rad * 180.0f / 3.1415f;
	if (direction < 0.0f) {
		direction += 360.0f;
	}

	position.setX(reader.readFloat() + 520000.0f);
	position.setY(reader.readFloat() + 520000.0f);
	z = reader.readFloat();

	for (uint8_t i = 0; i < 3; i++) {
		scaling[i] = reader.readFloat();
	}
}

void IFOEntry::readSpecialized(FileReader& reader) {

}

IFOMonsterSpawnEntry::IFOMonsterSpawnEntry(FileReader& reader) : IFOEntry(reader) {
	readSpecialized(reader);
}

IFOMonsterSpawnEntry::~IFOMonsterSpawnEntry() {

}

void IFOMonsterSpawnEntry::readSpecialized(FileReader& reader) {
	uint8_t strLen = reader.readByte();
	spawnName = reader.readString(strLen);
	readRounds(reader, basicRounds, false);
	readRounds(reader, tacticalRounds, true);
	respawnInterval = reader.readUInt();
	maximumAmountOfMonsters = reader.readUInt();
	maximumRadiusForSpawn = static_cast<float>(reader.readUInt()) * 100.0f;
	tacticalPointsNecessary = reader.readUInt();
}
	
void IFOMonsterSpawnEntry::readRounds(FileReader& reader, std::vector<std::shared_ptr<Round>>& roundVector, bool isTactical) {
	uint32_t amountOfSpawns = reader.readUInt();
	for (uint32_t j = 0; j < amountOfSpawns; j++) {
		uint8_t spawnNameLen = reader.readByte();
		auto spawnName = reader.readString(spawnNameLen);
		uint32_t monsterId = reader.readUInt();
		uint32_t amount = reader.readUInt() + 1;
		std::shared_ptr<Round> round = std::shared_ptr<Round>(new Round(monsterId, amount, isTactical));
		roundVector.push_back(round);
	}
}

IFONPCLocationEntry::IFONPCLocationEntry(FileReader& reader) : IFOEntry(reader) {
	readSpecialized(reader);
}

IFONPCLocationEntry::~IFONPCLocationEntry() {

}

void IFONPCLocationEntry::readSpecialized(FileReader& reader) {
	aiId = reader.readUInt();
	uint8_t strLen = reader.readByte();
	conFile = reader.readString(strLen);
}