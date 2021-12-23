#ifndef __ROSE_DROPS__
#define __ROSE_DROPS__

#include "../BasicTypes/Entity.h"
#include "../BasicTypes/Item.h"

class Drop : public Entity {
private:
	Item item;
	Entity* dropOwner;
	std::shared_ptr<char> name;
public:
	Drop(const Item& item, const Position& pos);
	virtual ~Drop();

	const Item& getItem() const {
		return item;
	}
	Entity* getDropOwner() const {
		return dropOwner;
	}
	virtual const char* getName() const;

	__inline virtual bool isDrop() const {
		return true;
	}

	virtual bool isItemDrop() const {
		return !isMoneyDrop();
	}
	virtual bool isMoneyDrop() const {
		return item.getType() == ItemTypeList::MONEY;
	}
};

#endif //__ROSE_DROPS__