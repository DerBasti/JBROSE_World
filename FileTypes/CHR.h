#ifndef __ROSE_CHR_FILETYPE__
#define __ROSE_CHR_FILETYPE__

#include <cstdint>
#include <memory>
#include "../../JBROSE_Common/Logger.h"
#include <map>
#include <vector>

enum class CHRMotionType : uint8_t {
	WARNING,
	WALKING,
	ATTACK,
	HIT,
	DYING,
	RUNNING,
	STOP,
	SKILL,
	CASTING,
	AMOUNT
};

class NPCChrAnimation {
private:
	uint16_t animationType;
	uint16_t animationId;
public:
	constexpr NPCChrAnimation() : NPCChrAnimation(0, 0) { }
	constexpr NPCChrAnimation(uint16_t id, uint16_t type) : animationId(id), animationType(type) { }
	virtual ~NPCChrAnimation() { }

	__inline uint16_t getAnimationId() const {
		return animationId;
	}
	__inline uint16_t getAnimationType() const {
		return animationType;
	}
};

class NPCCHRData {
private:
	uint16_t id;
	uint16_t idInChr;
	bool enabledFlag;
	std::shared_ptr<char> name;
	std::vector<NPCChrAnimation> animationData;
public:
	NPCCHRData() : NPCCHRData(0, false) { }
	NPCCHRData(uint16_t id, bool enabled);
	virtual ~NPCCHRData();

	__inline bool isNpcEnabled() const {
		return enabledFlag;
	}
	__inline const char* getName() const {
		return name.get();
	}
	__inline void setName(std::shared_ptr<char> newName) {
		name = newName;
	}
	__inline const std::vector<NPCChrAnimation>& getAnimationData() const {
		return animationData;
	}

	__inline uint16_t getAnimationIdOfType(CHRMotionType type) {
		return animationData.at(static_cast<uint8_t>(type)).getAnimationId();
	}

	__inline void addAnimationData(NPCChrAnimation data) {
		animationData.emplace_back(std::move(data));
	}
};

class CHRFile {
private:
	ROSELogger logger;
	void loadFileContent(class FileReader& reader);
	std::map<uint16_t, NPCCHRData*> npcChrData;
	std::map<uint16_t, class ZMO*>* animationsTypedMap;
	std::vector<class ZMO*> loadedZmoAnimations;
public:
	CHRFile(const char* filePath);
	virtual ~CHRFile();

	NPCCHRData* getChrDataFromId(const uint16_t id) const {
		auto iterator = npcChrData.find(id);
		if (iterator != npcChrData.cend()) {
			return iterator->second;
		}
		return nullptr;
	}
	class ZMO* getAttackAnimationForNpcId(const uint16_t id) const;
};

#endif //__ROSE_CHR_FILETYPE__