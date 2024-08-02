#include <string>

class ShoppingItems {
public: 
	std::string itemNames;
	int quantity; 
	float price;

	ShoppingItems(std::string p_itemNames, int p_quantity, float p_price) 
		: itemNames{ p_itemNames }, quantity(p_quantity), price{ p_price }
	{

	}
	ShoppingItems() = default;
};
