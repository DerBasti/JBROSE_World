#ifndef __ROSE_OPERATION_HANDLER__
#define __ROSE_OPERATION_HANDLER__

#include <cstdint>
#include <map>

enum class CheckOperationTypeId : uint8_t {
	EQUAL,
	BIGGER,
	BIGGER_EQUAL,
	SMALLER,
	SMALLER_EQUAL,
	NOT_EQUAL = 10,
	UNKNOWN = 11
};

class CheckOperationType {
private:
	CheckOperationTypeId typeId;
	const char* name;
public:
	const static CheckOperationType EQUAL;
	const static CheckOperationType BIGGER;
	const static CheckOperationType BIGGER_EQUAL;
	const static CheckOperationType SMALLER;
	const static CheckOperationType SMALLER_EQUAL;
	const static CheckOperationType NOT_EQUAL;
	const static CheckOperationType UNKNOWN;

	constexpr CheckOperationType(CheckOperationTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr CheckOperationType() : CheckOperationType(CheckOperationTypeId::UNKNOWN, "") {

	}
	virtual ~CheckOperationType() {

	}
	operator CheckOperationTypeId() const {
		return typeId;
	}
	__inline CheckOperationTypeId getTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
};


enum class ResultOperationTypeId : uint8_t {
	RETURN_OTHER_VALUE = 5,
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	INCREMENTATION,
	UNKNOWN = 11
};

class ResultOperationType {
private:
	ResultOperationTypeId typeId;
	const char* name;
public:
	const static ResultOperationType RETURN_OTHER_VALUE;
	const static ResultOperationType ADDITION;
	const static ResultOperationType SUBTRACTION;
	const static ResultOperationType MULTIPLICATION;
	const static ResultOperationType INCREMENTATION;
	const static ResultOperationType UNKNOWN;

	constexpr ResultOperationType(ResultOperationTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr ResultOperationType() : ResultOperationType(ResultOperationTypeId::UNKNOWN, "") {

	}
	virtual ~ResultOperationType() {

	}
	operator ResultOperationTypeId() const {
		return typeId;
	}
	__inline ResultOperationTypeId getTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
};

enum class AIPAbilityTypeId : uint8_t {
	LEVEL,
	ATTACK_POWER,
	DEFENSE,
	MAGIC_DEFENSE,
	CHARM,
	UNKNOWN
};

class AIPAbilityType {
private:
	AIPAbilityTypeId typeId;
	const char* name;
public:
	const static AIPAbilityType LEVEL;
	const static AIPAbilityType ATTACK_POWER;
	const static AIPAbilityType DEFENSE;
	const static AIPAbilityType MAGIC_DEFENSE;
	const static AIPAbilityType CHARM;
	const static AIPAbilityType UNKNOWN;

	constexpr AIPAbilityType(AIPAbilityTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr AIPAbilityType() : AIPAbilityType(AIPAbilityTypeId::UNKNOWN, "") { }
	virtual ~AIPAbilityType() { }

	__inline operator AIPAbilityTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline AIPAbilityTypeId getTypeId() const {
		return typeId;
	}
};


enum class EntityAbilityTypeId : uint8_t {
	BIRTHSTONE_TYPE = 3,
	JOB,
	STRENGTH = 10,
	DEXTERITY,
	INTELLIGENCE,
	CONCENTRATION,
	CHARM,
	SENSE,
	CURRENT_HP,
	CURRENT_MP,
	ATTACK_POWER,
	DEFENSE,
	ACCURACY,
	MAGIC_RESISTENCE,
	DODGE_RATE,
	MOVEMENT_SPEED,
	ATTACK_SPEED,
	INVENTORY_CAPACITY,
	CRITICAL_RATE,
	HP_RECOVERY_RATE,
	MP_RECOVERY_RATE,
	MP_CONSUMPTION,
	EXP_RATE,
	LEVEL,
	POINT,
	TENDENCY,
	PK_LEVEL,
	HEAD_SIZE,
	BODY_SIZE,
	SKILL_POINTS,
	MAX_HP,
	MAX_MP,
	MONEY,
	PLANET_REQUIREMENT = 75,
	STAMINA,
	NO_STORAGE_CHARGE = 94,
	STORAGE_EXPANSION,
	PERSONAL_SHOP_REMODELLING,
	CART_GAUGE,
	UNKNOWN = 255
};

class EntityAbilityType {
private:
	EntityAbilityTypeId typeId;
	const char* name;
public:
	const static EntityAbilityType BIRTHSTONE_TYPE;
	const static EntityAbilityType JOB;
	const static EntityAbilityType STRENGTH;
	const static EntityAbilityType DEXTERITY;
	const static EntityAbilityType INTELLIGENCE;
	const static EntityAbilityType CONCENTRATION;
	const static EntityAbilityType CHARM;
	const static EntityAbilityType SENSE;
	const static EntityAbilityType CURRENT_HP;
	const static EntityAbilityType CURRENT_MP;
	const static EntityAbilityType ATTACK_POWER;
	const static EntityAbilityType DEFENSE;
	const static EntityAbilityType ACCURACY;
	const static EntityAbilityType MAGIC_RESISTENCE;
	const static EntityAbilityType DODGE_RATE;
	const static EntityAbilityType MOVEMENT_SPEED;
	const static EntityAbilityType ATTACK_SPEED;
	const static EntityAbilityType INVENTORY_CAPACITY;
	const static EntityAbilityType CRITICAL_RATE;
	const static EntityAbilityType HP_RECOVERY_RATE;
	const static EntityAbilityType MP_RECOVERY_RATE;
	const static EntityAbilityType MP_CONSUMPTION;
	const static EntityAbilityType EXP_RATE;
	const static EntityAbilityType LEVEL;
	const static EntityAbilityType SKILL_POINTS;
	const static EntityAbilityType MAX_HP;
	const static EntityAbilityType MAX_MP;
	const static EntityAbilityType MONEY;
	const static EntityAbilityType UNKNOWN;
	constexpr EntityAbilityType(EntityAbilityTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr EntityAbilityType() : EntityAbilityType(EntityAbilityTypeId::UNKNOWN, "") { }
	virtual ~EntityAbilityType() { }

	__inline operator EntityAbilityTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline EntityAbilityTypeId getTypeId() const {
		return typeId;
	}
};

class OperationHandler {
private:
	OperationHandler() { }
	virtual ~OperationHandler() { }
	static std::map<uint8_t, CheckOperationType> checkOperationTypeMap;
	static std::map<uint8_t, ResultOperationType> resultOperationTypeMap;
	static std::map<uint8_t, AIPAbilityType> abilityTypeMap;
	static std::map<uint8_t, EntityAbilityType> entityAbilityTypeMap;
public:
	static CheckOperationType fromCheckOperationTypeId(const uint8_t operationId) {
		return checkOperationTypeMap.at(operationId);
	}
	static ResultOperationType fromResultOperationTypeId(const uint8_t operationId) {
		return resultOperationTypeMap.at(operationId);
	}
	static AIPAbilityType fromAIPAbilityTypeId(const uint8_t abilityTypeId) {
		return abilityTypeMap.at(abilityTypeId);
	}
	static EntityAbilityType fromEntityAbilityTypeId(const uint8_t abilityTypeId) {
		auto it = entityAbilityTypeMap.find(abilityTypeId);
		if (it != entityAbilityTypeMap.end()) {
			return it->second;
		}
		return EntityAbilityType::UNKNOWN;
	}


	template<class _FirstType, class _SecondType>
	static bool executeCheckOperation(const _FirstType& left, const _SecondType& right, CheckOperationType operation) {
		switch (operation) {
			case CheckOperationTypeId::EQUAL:
				return left == right;
			case CheckOperationTypeId::BIGGER:
				return left > right;
			case CheckOperationTypeId::BIGGER_EQUAL:
				return left >= right;
			case CheckOperationTypeId::SMALLER:
				return left < right;
			case CheckOperationTypeId::SMALLER_EQUAL:
				return left <= right;
			case CheckOperationTypeId::NOT_EQUAL:
				return left != right;
		}
		return false;
	}
	template<class _Type> 
	static _Type executeResultOperation(const _Type& left, const _Type& right, ResultOperationType operation) {
		switch (operation) {
			case ResultOperationTypeId::RETURN_OTHER_VALUE:
				return right;
			case ResultOperationTypeId::ADDITION:
				return _Type(left + right);
			case ResultOperationTypeId::SUBTRACTION:
				return _Type(left - right);
			case ResultOperationTypeId::MULTIPLICATION:
				return _Type(left * right);
			case ResultOperationTypeId::INCREMENTATION:
				return _Type(left + 1);
		}
		return _Type(0);
	}
	static uint16_t getAIPAbilityValueOfEntity(class Entity* entity, AIPAbilityType type);
};

#endif //__ROSE_OPERATION_HANDLER__