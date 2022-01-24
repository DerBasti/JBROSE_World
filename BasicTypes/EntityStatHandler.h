#ifndef __ROSE_ENTITY_STAT_HANDLER__
#define __ROSE_ENTITY_STAT_HANDLER__

#include <functional>
#include "OperationHandler.h"

template<class ValueType>
class GetterAndSetterTuple {
private:
	std::function<ValueType()> getter;
	std::function<void(ValueType)> setter;
public:
	GetterAndSetterTuple() : GetterAndSetterTuple(std::function<ValueType()>(), std::function<void(ValueType)>()) {
	}
	GetterAndSetterTuple(std::function<ValueType()> getter, std::function<void(ValueType)> setter) {
		this->getter = getter;
		this->setter = setter;
	}
	virtual ~GetterAndSetterTuple() {

	}
	__inline bool isValid() const {
		return getter && setter;
	}
	__inline ValueType get() const {
		return getter();
	}
	__inline void set(ValueType value) {
		setter(value);
	}
	__inline std::function<void(ValueType)> getSetterMethod() const {
		return setter;
	}
};

class EntityStatHandler {
public:
	template<class _Numeric, class = typename std::enable_if<std::is_integral<_Numeric>::value || std::is_floating_point<_Numeric>::value>::type>
	static GetterAndSetterTuple<_Numeric> getNumericGetterAndSetterOfEntityByEntityAbilityType(class Entity* entity, EntityAbilityType type);

	template<class _Numeric, class = typename std::enable_if<std::is_integral<_Numeric>::value || std::is_floating_point<_Numeric>::value>::type>
	static GetterAndSetterTuple<_Numeric> getNumericGetterAndSetterOfPlayerByEntityAbilityType(class Player* entity, EntityAbilityType type);
};

#include "EntityStatHandler.tpp"


#endif //