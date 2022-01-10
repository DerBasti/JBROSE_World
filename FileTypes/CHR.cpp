#include "CHR.h"
#include "../../JBROSE_Common/FileReader.h"
#include "ZMO.h"

CHRFile::CHRFile(const char* filePath) {
	animationsTypedMap = new std::map<uint16_t, class ZMO*>[static_cast<uint8_t>(CHRMotionType::AMOUNT)];
	FileInputReader reader(filePath);
	if (reader.isValid()) {
		loadFileContent(reader);
	}
}

CHRFile::~CHRFile() {
	for (ZMO *zmo : loadedZmoAnimations) {
		delete zmo;
		zmo = nullptr;
	}
	for (std::pair<uint16_t, NPCCHRData*> dataPair : npcChrData) {
		delete dataPair.second;
		dataPair.second = nullptr;
	}
	npcChrData.clear();

	delete[] animationsTypedMap;
}

void CHRFile::loadFileContent(FileReader& reader) {
	logger.logInfo("Starting to load NPC animation information...");

	uint16_t skeletonFileAmount = reader.readUShort();
	for (uint16_t currentSkeletonFileId = 0; currentSkeletonFileId < skeletonFileAmount; currentSkeletonFileId++) {
		auto result = reader.readStringWrapped();
	}
	uint16_t animationFileAmount = reader.readUShort();
	logger.logDebug("A total of ", animationFileAmount, " animations are stored.");
	std::vector<ZMO*> animations;
	std::string basicPath = "D:\\Games\\ROSE Server\\VFS_Extrator\\";
	for (uint16_t i = 0; i < animationFileAmount; i++) {
		std::shared_ptr<char> subPath = reader.readStringWrapped();
		std::string totalPath = basicPath + std::string(subPath.get());
		logger.logTrace("Reading monster animation from: ", totalPath.c_str());
		ZMO* zmo = new ZMO(totalPath.c_str());
		animations.push_back(zmo);
	}
	uint16_t effectAmount = reader.readUShort();
	for (uint16_t i = 0; i < effectAmount; i++) {
		reader.readStringWrapped();
	}
	uint16_t npcAmount = reader.readUShort();
	logger.logDebug("Found a total of ", npcAmount, " NPC entries.");
	for (uint16_t i = 0; i < npcAmount; i++) {
		bool isNpcEnabled = reader.readByte() > 0;
		NPCCHRData* data = new NPCCHRData(i, isNpcEnabled);
		if (!data->isNpcEnabled()) {
			npcChrData.emplace(i, data);
			continue;
		}
		uint16_t readId = reader.readUShort();
		data->setName(reader.readStringWrapped());
		uint16_t objectAmount = reader.readUShort();
		if (objectAmount > 0) {
			reader.skipBytes(sizeof(uint16_t) * objectAmount);
		}
		uint16_t animationAmount = reader.readUShort();
		for (uint16_t j = 0; j < animationAmount; j++) {
			uint16_t animationType = reader.readUShort();
			uint16_t animationId = reader.readUShort();
			if (animationType >= static_cast<uint16_t>(CHRMotionType::AMOUNT)) {
				logger.logDebug("Animation type '", animationType, "' is not valid!");
				continue;
			}
			this->animationsTypedMap[animationType].emplace(i, animations.at(animationId));
			data->addAnimationData(std::move(NPCChrAnimation(animationId, animationType)));
		}
		uint16_t effectCount = reader.readUShort();
		if (effectCount > 0) {
			reader.skipBytes(sizeof(uint16_t) * 2 * effectCount);
		}
		npcChrData.emplace(i, data);
	}
	logger.logInfo("Finished loading NPC animation information.");
}

ZMO* CHRFile::getAttackAnimationForNpcId(const uint16_t id) const {
	auto chrData = getChrDataFromId(id);
	if (chrData != nullptr) {
		return animationsTypedMap[static_cast<uint16_t>(CHRMotionType::ATTACK)].at(id);
	}
	return nullptr;
}

NPCCHRData::NPCCHRData(uint16_t id, bool enabled) {
	this->id = id;
	this->enabledFlag = enabled;
}

NPCCHRData::~NPCCHRData() {

}