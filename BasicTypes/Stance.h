#ifndef __ROSE_STANCE__
#define __ROSE_STANCE__

#include <functional>

class Stance {
private:
	uint8_t stanceId;
	std::function<void()> stanceUpdateFunction;
	virtual void onStanceChange(uint8_t newStance) {
		if (newStance != stanceId) {
			stanceId = newStance;
			stanceUpdateFunction();
		}
	}
protected:
	const static uint8_t INVALID_STANCE = -1;

	virtual uint8_t getSittingStanceId() const {
		return INVALID_STANCE;
	}
	virtual uint8_t getWalkingStanceId() const {
		return INVALID_STANCE;
	}
	virtual uint8_t getRunningStanceId() const {
		return INVALID_STANCE;
	}
	virtual uint8_t getDrivingStanceId() const {
		return INVALID_STANCE;
	}
public:
	Stance(std::function<void()> visualAndSpeedUpdateFunction) {
		stanceUpdateFunction = std::move(visualAndSpeedUpdateFunction);
	}
	virtual ~Stance() {}
	__inline void setStanceById(const uint8_t stanceId) {
		onStanceChange(stanceId);
	}
	__inline bool isSitting() const {
		return getStanceId() != INVALID_STANCE && getStanceId() == getSittingStanceId();
	}
	void setSittingStance() {
		onStanceChange(getSittingStanceId());
	}
	__inline bool isWalking() const {
		return getStanceId() != INVALID_STANCE && getStanceId() == getWalkingStanceId();
	}
	void setWalkingStance() {
		onStanceChange(getWalkingStanceId());
	}
	__inline bool isRunning() const {
		return getStanceId() != INVALID_STANCE && getStanceId() == getRunningStanceId();;
	}
	void setRunningStance() {
		onStanceChange(getRunningStanceId());
	}
	__inline bool isDriving() const {
		return getStanceId() != INVALID_STANCE && getStanceId() == getDrivingStanceId();
	}
	void setDrivingStance() {
		onStanceChange(getDrivingStanceId());
	}
	__inline uint8_t getStanceId() const {
		return stanceId;
	}
};

class PlayerStance : public Stance {
private:
	const static uint8_t SITTING = 0x01;
	const static uint8_t WALKING = 0x02;
	const static uint8_t RUNNING = 0x03;
	const static uint8_t DRIVING = 0x04;
protected:
	virtual uint8_t getSittingStanceId() const {
		return SITTING;
	}
	virtual uint8_t getWalkingStanceId() const {
		return WALKING;
	}
	virtual uint8_t getRunningStanceId() const {
		return RUNNING;
	}
	virtual uint8_t getDrivingStanceId() const {
		return DRIVING;
	}
public:
	PlayerStance(std::function<void()> visualAndSpeedUpdateFunction) : Stance(visualAndSpeedUpdateFunction) {
	}
	virtual ~PlayerStance() {}
};

class NPCStance : public Stance {
private:
	const static uint8_t WALKING = 0x00;
	const static uint8_t RUNNING = 0x01;
protected:
	virtual uint8_t getWalkingStanceId() const {
		return WALKING;
	}
	virtual uint8_t getRunningStanceId() const {
		return RUNNING;
	}
public:
	NPCStance(std::function<void()> visualAndSpeedUpdateFunction) : Stance(visualAndSpeedUpdateFunction) {

	}
	virtual ~NPCStance() {}
};

#endif //__ROSE_STANCE__