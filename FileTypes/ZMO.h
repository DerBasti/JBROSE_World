#ifndef __ROSE_ZMO__
#define __ROSE_ZMO__

#include "../../JBROSE_Common/FileStoredReader.h"
#include "../../JBROSE_Common/WrappingNumeric.h"
#include "STB.h"

class ZMOFrame {
private:
	uint32_t triggerTimepoint;
	uint16_t frameType;
public:
	ZMOFrame() : ZMOFrame(0, 0) {
	
	}
	ZMOFrame(uint32_t triggerTimepointInMillis, uint16_t frameTriggerType) {
		this->triggerTimepoint = triggerTimepointInMillis;
		this->frameType = frameTriggerType;
	}
	virtual ~ZMOFrame() {

	}
	__inline uint32_t getTriggerTimepointInMillis() const {
		return triggerTimepoint;
	}
	__inline uint32_t getFrameType() const {
		return frameType;
	}
};

class ZMOFrameContainer {
private:
	ZMOFrame* frames;
	uint32_t frameAmount;
	WrappingUInt frameIndex;
public:
	ZMOFrameContainer();
	ZMOFrameContainer(std::vector<uint32_t> times, std::vector<uint16_t> types);
	ZMOFrameContainer(ZMOFrameContainer&) = delete;
	virtual ~ZMOFrameContainer();

	ZMOFrameContainer& operator=(const ZMOFrameContainer& other);
	ZMOFrameContainer& operator++() {
		frameIndex++;
		return (*this);
	}
	ZMOFrameContainer& operator++(int) {
		frameIndex++;
		return (*this);
	}
	__inline bool hasFrames() const {
		return frameAmount > 0;
	}
	ZMOFrame* getAllFrames() const {
		return frames;
	}
	const ZMOFrame& getTriggerFrame() const {
		return frames[frameIndex];
	}
};

class ZMO {
private:
	std::string filePath;
	ROSELogger logger;
	uint32_t framesPerSecond;
	uint32_t totalAmountOfFrames;
	uint32_t defaultPlayoutTime;
	ZMOFrameContainer frameContainer;
public:
	ZMO(const char* filePath);

	virtual ~ZMO() {

	}
	__inline std::string getFilePath() const {
		return filePath;
	}
	__inline bool isAttackAnimation() const {
		return frameContainer.hasFrames();
	}
	__inline uint32_t getDefaultPlayTime() const {
		return defaultPlayoutTime;
	}
	__inline ZMOFrameContainer& getAttackFrames() {
		return frameContainer;
	}
};

#endif //__ROSE_ZMO__