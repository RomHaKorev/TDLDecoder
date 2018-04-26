#include "basedataelement.h"




std::ostream& operator<<(std::ostream& os, const L16::IDataElement& elt)
{
	os << elt.name().c_str() << ": " << elt.value();
	return os;
}


uint1024_t const& operator>>(uint1024_t const& data, L16::IDataElement& elt)
{
	elt(data);
	return data;
}


using json = nlohmann::json;

nlohmann::json L16::IDataElement::to_json() const
{
    json j = R"({"compact": true, "schema": 0})"_json;
    return j;
}


nlohmann::json const& operator>>(nlohmann::json const& data, L16::IDataElement& elt)
{
    elt = data;
    return data;
}


uint1024_t& operator<<(uint1024_t& data, const L16::IDataElement& elt)
{
//    std::cout << elt.name() << std::endl;
//    std::cout << std::hex << "Orig: " << data << std::endl;
    uint1024_t mask = ~(elt.mask());
//    std::cout << std::hex << "mask: " << mask << std::endl;
//    std::cout << std::hex << "Elt val: " << (elt.value() << elt.position()) << std::endl;
//    std::cout << std::hex << "Elt pos: " << std::dec << elt.position() << std::endl;
    uint1024_t value = (uint1024_t(elt.value()) << elt.position());
//    std::cout << std::hex << "Value: " << value << std::endl;
    data = (data & mask) | value;
//    std::cout << "----------" << std::endl;
    return data;
}
