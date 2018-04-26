#ifndef BASE_DATA_ELEMENT_H
#define BASE_DATA_ELEMENT_H


#include <boost/multiprecision/cpp_int.hpp>
#include "json.hpp"


using namespace boost::multiprecision;

namespace L16 __attribute__((visibility ("hidden"))) {

template <class T>
constexpr T make_mask(std::size_t pos, std::size_t len)
{
    return ((static_cast<T>(1) << len) - 1) << pos;
}


class IDataElement
{
public:
    IDataElement(): m_value(-1)
    {}
    virtual std::string name() const = 0;
    virtual bool is_technical() const = 0;
    unsigned long value() const { return m_value; }
    virtual unsigned int position() const = 0;
    bool is_valid() const { return m_validity; }
    virtual bool operator() (uint1024_t const& data) = 0;
    virtual void operator=(nlohmann::json const& data) = 0;

    virtual uint1024_t mask() const = 0;

    std::string identifier() const { return name(); }

    nlohmann::json to_json() const;

    enum Type {
        Technical = 0,
        Tactical = 1,
        Auto = 2
    };

	//friend uint1024_t& operator<<(uint1024_t& data, const L16::IDataElement& elt);

protected:
    unsigned long m_value;
    bool m_validity;
};

template <unsigned int dfi, unsigned int dui, unsigned int pos, unsigned int length, unsigned long ns=-1, IDataElement::Type type=IDataElement::Tactical> class DataElement: public IDataElement
{
public:
    DataElement()
    {
        m_value = ns;
    }

    bool operator() (uint1024_t const& data) override
    {
        m_value = static_cast <unsigned int>((data & mask()) >> pos);
        m_validity = true;
        return true;
    }

    void operator=(nlohmann::json const& data) override
    {
        if (type != IDataElement::Technical)
        {
            m_value = data.value(name(), (unsigned long)(-1));
            m_validity = (m_value != (unsigned long)(-1));
        }
    }

    uint1024_t const& operator >>(uint1024_t const & data)
    {
        m_value = static_cast <unsigned int>((data & mask()) >> pos);
        m_validity = true;
        return data;
    }

    uint1024_t mask() const
    {
        return (make_mask<uint1024_t>(pos, length));
    }

    unsigned int position() const override
    {
        return pos;
    }

    bool is_technical() const override { return type != IDataElement::Tactical; }

    template <unsigned int, unsigned int>
    friend uint1024_t const& operator>>(uint1024_t const& data, IDataElement& elt);

protected:
    uint1024_t m_mask;
};

}
template <unsigned int pos, unsigned int length, unsigned long ns>
uint1024_t const& operator>>(uint1024_t const& data, L16::IDataElement& elt)
{
    elt(data);
    return data;
}

uint1024_t& operator<<(uint1024_t& data, const L16::IDataElement& elt);
std::ostream& operator<<(std::ostream& os, const L16::IDataElement& elt);
uint1024_t const& operator>>(uint1024_t const& data, L16::IDataElement& elt);
nlohmann::json const& operator>>(nlohmann::json const& data, L16::IDataElement& elt);

#endif // BASE_DATA_ELEMENT_H_16392080208
