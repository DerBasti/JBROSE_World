#ifndef __ROSE_COMBAT__
#define __ROSE_COMBAT__

#include "../../JBROSE_Common/Timer.h"
#include "EntityStats.h"
#include "../../JBROSE_Common/Logger.h"
#include "../../JBROSE_Common/Randomizer.h"
#include <map>

enum class EntityDefaultTeamId : uint16_t {
	NPC = 0x01,
	MONSTER = 0x02,
	PLAYER = 0x64,
	DROP = PLAYER
};

class DamageHit {
private:
	const static uint16_t STUNLOCK_HIT_FLAG = 0x2000;
	const static uint16_t CRIT_HIT_FLAG = 0x4000;
	const static uint16_t DEADLY_HIT_FLAG = 0x8000;

	uint16_t amount;
	uint16_t flag;
	class Entity* attacker;
	class Entity* target;
public:
	DamageHit() : DamageHit(nullptr, nullptr, 0) {}
	DamageHit(Entity* attacker, Entity* target, uint16_t damageAmount) {
		this->attacker = attacker;
		this->target = target;
		amount = damageAmount;
		flag = 0;
	}
	virtual ~DamageHit() {

	}
	__inline Entity* getAttacker() const {
		return attacker;
	}
	__inline Entity* getTarget() const {
		return target;
	}
	__inline uint16_t getDamageAmount() const {
		return amount;
	}
	__inline uint16_t getHitFlag() const {
		return flag;
	}
	__inline void setDeadlyHitFlag() {
		flag |= DEADLY_HIT_FLAG;
	}
	__inline bool isDeadlyHit() const {
		return flag & DEADLY_HIT_FLAG;
	}
	__inline void setCriticalFlag() {
		flag |= CRIT_HIT_FLAG;
	}
};

class CombatTimer : public WrappingTimer {
public:
	using CombatTimerMethod = std::function<void()>;
private:
	uint64_t timeInMillisTillProc;
	bool hasProcced;
	CombatTimerMethod procMethod;
protected:
	__inline uint64_t getDurationForProcInMillis() const {
		return timeInMillisTillProc;
	}
public:
	CombatTimer() : CombatTimer(-1, -1) {}
	CombatTimer(uint64_t wrapTimeInMillis, uint64_t timeInMillisTillProc) : CombatTimer(wrapTimeInMillis, timeInMillisTillProc, nullptr) {}
	CombatTimer(uint64_t wrapTimeInMillis, uint64_t timeInMillisTillProc, CombatTimerMethod procFunction) : WrappingTimer(wrapTimeInMillis) {
		this->timeInMillisTillProc = timeInMillisTillProc;
		hasProcced = false;
		setCombatProcMethod(procFunction);
	}
	virtual ~CombatTimer() {}

	virtual uint64_t updateTimestamp() {
		uint64_t timePassed = getPassedTimeInMillis();
		if (timePassed >= timeInMillisTillProc && !hasProcced && procMethod) {
			onProc();
			procMethod();
			hasProcced = true;
		}
		WrappingTimer::updateTimestamp();
		return timePassed;
	}

	virtual void onProc() {
	}

	virtual void onTimeWrap() {
		hasProcced = false;
	}

	__inline void setCombatProcMethod(CombatTimerMethod procMethod) {
		this->procMethod = procMethod;
	}
	__inline void setDurationForProcInMillis(const uint64_t procTime) {
		this->timeInMillisTillProc = procTime;
	}
};

class StoppableCombatTimer : public CombatTimer {
private:
	bool stoppedFlag;
	bool softstopFlag;
public:
	StoppableCombatTimer() : StoppableCombatTimer(-1, -1) {}
	StoppableCombatTimer(uint64_t wrapTimeInMillis, uint64_t timeInMillisTillProc) : StoppableCombatTimer(wrapTimeInMillis, timeInMillisTillProc, nullptr) {}
	StoppableCombatTimer(uint64_t wrapTimeInMillis, uint64_t timeInMillisTillProc, CombatTimerMethod procFunction) : CombatTimer(wrapTimeInMillis, timeInMillisTillProc, procFunction) {
		stoppedFlag = softstopFlag = false;
	}
	__inline bool isStopped() const {
		return stoppedFlag;
	}
	__inline bool isRunning() const {
		return !isStopped();
	}
	__inline void softStop() {
		softstopFlag = true;
	}
	__inline void hardStop() {
		stoppedFlag = softstopFlag = true;
	}
	__inline bool isSoftStopped() const {
		return softstopFlag;
	}
	__inline void restart() {
		stoppedFlag = softstopFlag = false;
		setTimestampToCurrentTimepoint();
	}
	virtual uint64_t updateTimestamp() {
		if (isStopped()) {
			return 0;
		}
		uint64_t timePassed = getPassedTimeInMillis();
		if (isSoftStopped() && timePassed >= getDurationForWrappingInMillis()) {
			stoppedFlag = true;
		}
		return CombatTimer::updateTimestamp();
	}
};

enum class CombatTypeId : uint8_t {
	NONE,
	BASIC_ATTACK,
	SKILL_ATTACK,
	AMOUNT_OF_ENTRIES
};

class CombatType {
private:
	CombatTypeId typeId;
	std::function<void()> combatTypeSwitchCallback;
	CombatType(CombatTypeId id) : typeId(id) {
		combatTypeSwitchCallback = nullptr;
	}
public:
	const static CombatType NONE;
	const static CombatType BASIC_ATTACK;
	const static CombatType SKILL_ATTACK;
	CombatType();
	explicit CombatType(const CombatType&) = default;
	virtual ~CombatType();
	explicit operator uint8_t() {
		return static_cast<uint8_t>(typeId);
	}
	explicit operator uint8_t() const {
		return static_cast<uint8_t>(typeId);
	}
	operator CombatTypeId() {
		return typeId;
	}
	operator CombatTypeId() const {
		return typeId;
	}
	CombatType& operator=(const CombatType& other) {
		if (typeId != other.typeId && combatTypeSwitchCallback) {
			combatTypeSwitchCallback();
		}
		typeId = other.typeId;
		return (*this);
	}
	CombatTypeId getTypeId() const {
		return typeId;
	}
	__inline void setCombatTypeSwitchCallback(std::function<void()> callback) {
		combatTypeSwitchCallback = callback;
	}
};

std::ostream& operator<<(std::ostream& out, const CombatType& type);
std::wostream& operator<<(std::wostream& out, const CombatType& type);
std::ostream& operator<<(std::ostream& out, const CombatTypeId& type);
std::wostream& operator<<(std::wostream& out, const CombatTypeId& type);

class Combat {
private:
	class Entity *owner;
	class Entity *target;
	class ZMO* attackAnimation;
	std::map<uint16_t, class Entity*> targetedByMap;
	CombatType combatType;
	StoppableCombatTimer attackTimer;
	uint32_t teamId;
	ROSELogger logger;
	NumericRandomizer<uint32_t> randomizer;

	void setAttackRoutine(std::function<bool()> proc);
	float getAttackTimeInMilliseconds() const;
	float getAttackTimeRatio() const;

	void addToTargetedByList(Entity* entity);
	void removeFromTargetedByList(Entity* entity);
public:
	Combat(Entity* combatOwner);
	virtual ~Combat();

	void clear();
	__inline void refresh() {
		attackTimer.updateTimestamp();
	}

	DamageHit doBasicAttack();
	bool isTargetInReach() const;
	void onMovementUpdate();
	void onAttackspeedUpdate();
	void onTargetReached();

	__inline Entity* getEntitySelf() const {
		return owner;
	}
	__inline Entity* getTarget() const {
		return target;
	}
	__inline uint32_t getTeamId() const {
		return teamId;
	}
	__inline void setTeamId(const uint32_t id) {
		teamId = id;
	}
	__inline void setTeamId(const EntityDefaultTeamId& id) {
		setTeamId(static_cast<uint32_t>(id));
	}
	__inline const CombatType& getCombatType() const {
		return combatType;
	}
	__inline void setAttackAnimation(ZMO* zmo) {
		this->attackAnimation = zmo;
	}

	__inline const std::map<uint16_t, class Entity*>& getTargetedByEnemiesList() const {
		return targetedByMap;
	}


	__inline bool isAttackRunning() {
		attackTimer.updateTimestamp();
		return attackTimer.isRunning();
	}

	void setTarget(Entity* target, const CombatType& type);
	void clearSelfFromTargetsCombat();
};


#endif //__ROSE_COMBAT__