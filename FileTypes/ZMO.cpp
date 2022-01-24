#include "ZMO.h"

ZMO::ZMO(const char* filePath) {
	this->filePath = std::string(filePath);
	framesPerSecond = totalAmountOfFrames = defaultPlayoutTime = 0;
	FileInputReader fir(filePath);
	if (fir.isValid()) {
		auto str = fir.readStringWrapped(8);
		if (_stricmp(str.get(), "ZMO0002") == 0) {
			framesPerSecond = fir.readUInt();
			totalAmountOfFrames = fir.readUInt();
			defaultPlayoutTime = totalAmountOfFrames * 1000 / framesPerSecond;
			fir.resetCaretTo(fir.getFileSize() - 14 - (sizeof(uint16_t)* totalAmountOfFrames));
			std::vector<uint32_t> times;
			std::vector<uint16_t> types;
			for (uint32_t currentFrame = 0; currentFrame < totalAmountOfFrames; currentFrame++) {
				uint16_t frameType = fir.readUShort();
				if (currentFrame < 4 || (frameType < 31 || frameType>39)) {
					continue;
				}
				uint32_t previousFrame = currentFrame - 2;
				uint32_t timePoint = previousFrame * 1000 / framesPerSecond;
				logger.logTrace("Found an attack animation (#", frameType, ") at timepoint: ", timePoint, "ms.");
				times.push_back(timePoint);
				types.push_back(frameType);
			}
			if (!times.empty() && !types.empty()) {
				logger.logTrace("Found a total of ", times.size(), " attack animations for ZMO: '", filePath, "'.");
				frameContainer = ZMOFrameContainer(times, types);
			}
		}
	}
}

ZMOFrameContainer::ZMOFrameContainer() : ZMOFrameContainer(std::vector<uint32_t>(), std::vector<uint16_t>()) {
	frames = nullptr;
}

ZMOFrameContainer::ZMOFrameContainer(std::vector<uint32_t> times, std::vector<uint16_t> types) {
	if (times.size() != types.size()) {
		throw;
	}
	frameAmount = times.size();
	frameIndex = 0;
	frames = new ZMOFrame[frameAmount + 1];
	for (uint32_t i = 0; i < times.size(); i++) {
		frames[i] = ZMOFrame(times.at(i), types.at(i));
	}
}

ZMOFrameContainer::~ZMOFrameContainer() {
	delete[] frames;
	frames = nullptr;
}
ZMOFrameContainer& ZMOFrameContainer::operator=(const ZMOFrameContainer& other) {
	if (frames) {
		delete[] frames;
	}
	frameAmount = other.frameAmount;
	frameIndex = 0;
	frames = new ZMOFrame[other.frameAmount + 1];
	for (uint32_t i = 0; i < frameAmount; i++) {
		frames[i] = other.frames[i];
	}
	return (*this);
}