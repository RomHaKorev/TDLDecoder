#include "basemessage.h"

namespace L16 __attribute__((visibility ("hidden"))) {

template <class T> T generator()
{
    return T();
}

std::vector<uint8_t>& operator<<(std::vector<uint8_t>& vec, IBaseMessage const& elt)
{
    elt.to_bin(vec);
    return vec;
}

nlohmann::json& operator<<(nlohmann::json& j, IBaseMessage const& elt)
{
    j = elt.to_json();
    return j;
}

static nlohmann::json flatten(nlohmann::json const& json)
{
    nlohmann::json json_flat;
    return json_flat;
}

}
