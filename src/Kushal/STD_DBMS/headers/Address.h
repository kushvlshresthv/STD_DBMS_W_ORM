#include <string>
class Address {
public: 
	std::string state;
	std::string district;
	std::string city;

	Address(std::string pstate, std::string pdistrict, std::string pcity) 
		: state{pstate}, district {pdistrict }, city {pcity} 
	{

	}

	Address() = default;
};