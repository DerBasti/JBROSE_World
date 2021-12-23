#ifndef __ROSE_OPERATION_HANDLER__
#define __ROSE_OPERATION_HANDLER__

#include <cstdint>
#include <map>

enum class OperationTypeId : uint8_t {
	EQUAL,
	BIGGER,
	BIGGER_EQUAL,
	SMALLER,
	SMALLER_EQUAL,
	RETURN_OTHER_VALUE,
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	INCREMENTATION,
	NOT_EQUAL,
	INVALID
};

class OperationType {
private:
	OperationTypeId typeId;
	const char* name;
public:
	const static OperationType EQUAL;
	const static OperationType BIGGER;
	const static OperationType BIGGER_EQUAL;
	const static OperationType SMALLER;
	const static OperationType SMALLER_EQUAL;
	const static OperationType RETURN_OTHER_VALUE;
	const static OperationType ADDITION;
	const static OperationType SUBTRACTION;
	const static OperationType MULTIPLICATION;
	const static OperationType INCREMENTATION;
	const static OperationType NOT_EQUAL;

	constexpr OperationType(OperationTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr OperationType() : OperationType(OperationTypeId::INVALID, "") {

	}
	virtual ~OperationType() {

	}
	operator OperationTypeId() const {
		return typeId;
	}
	__inline OperationTypeId getTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
};

enum class AbilityTypeId : uint8_t {
	LEVEL,
	ATTACK_POWER,
	DEFENSE,
	MAGIC_DEFENSE,
	CHARM,
	UNKNOWN
};

class AbilityType {
private:
	AbilityTypeId typeId;
	const char* name;
public:
	const static AbilityType LEVEL;
	const static AbilityType ATTACK_POWER;
	const static AbilityType DEFENSE;
	const static AbilityType MAGIC_DEFENSE;
	const static AbilityType CHARM;
	const static AbilityType UNKNOWN;

	constexpr AbilityType(AbilityTypeId id, const char* newName) : typeId(id), name(newName) {

	}
	constexpr AbilityType() : AbilityType(AbilityTypeId::UNKNOWN, "") { }
	virtual ~AbilityType() { }

	__inline operator AbilityTypeId() const {
		return typeId;
	}
	__inline const char* getName() const {
		return name;
	}
	__inline AbilityTypeId getTypeId() const {
		return typeId;
	}
};


class OperationHandler {
private:
	OperationHandler() { }
	virtual ~OperationHandler() { }
	static std::map<uint8_t, OperationType> operationTypeMap;
	static std::map<uint8_t, AbilityType> abilityTypeMap;
public:
	static OperationType fromOperationTypeId(const uint8_t operationId) {
		return operationTypeMap.at(operationId);
	}
	static AbilityType fromAbilityTypeId(const uint8_t abilityTypeId) {
		return abilityTypeMap.at(abilityTypeId);
	}
	template<class _FirstType, class _SecondType>
	static bool executeCheckOperation(const _FirstType& left, const _SecondType& right, OperationType operation) {
		switch (operation) {
			case OperationTypeId::EQUAL:
				return left == right;
			case OperationTypeId::BIGGER:
				return left > right;
			case OperationTypeId::BIGGER_EQUAL:
				return left >= right;
			case OperationTypeId::SMALLER:
				return left < right;
			case OperationTypeId::SMALLER_EQUAL:
				return left <= right;
			case OperationTypeId::NOT_EQUAL:
				return left != right;
		}
		return false;
	}
	template<class _Type> 
	static _Type executeResultOperation(const _Type& left, const _Type& right, OperationType operation) {
		switch (operation) {
			case OperationTypeId::RETURN_OTHER_VALUE:
				return _Type(right);
			case OperationTypeId::ADDITION:
				return _Type(left + right);
			case OperationTypeId::SUBTRACTION:
				return _Type(left - right);
			case OperationTypeId::MULTIPLICATION:
				return _Type(left * right);
			case OperationTypeId::INCREMENTATION:
				return _Type(left + 1);
		}
		return _Type(0);
	}
	static uint16_t getAbilityValueOfEntity(class Entity* entity, AbilityType type);
};

#endif //__ROSE_OPERATION_HANDLER__