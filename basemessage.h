#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include <memory>
#include <vector>
#include <map>

#include "basecodeword.h"

namespace L16 __attribute__((visibility ("hidden"))) {

struct IBuilder {
    virtual ICodeword* build() const = 0;
    virtual bool match(uint1024_t const& data) const = 0;
    virtual std::string name() const = 0;
};

template <class T> class Builder: public IBuilder
{
    ICodeword* build() const override
    {
        return new T();
    }

    bool match(uint1024_t const& data) const override
    {
        return T::match(data);
    }

    std::string name() const override
    {
        return T::name();
    }
};


struct IBaseMessage {
    virtual std::string name() const = 0;
    virtual void to_bin(std::vector<std::uint8_t>& frame) const = 0;
    virtual nlohmann::json to_json(bool with_technical=false) const = 0;
    static nlohmann::json flatten();
};

template<unsigned int label, unsigned int subLabel, class ...Codeword> class Base_Jx_y_Message: public IBaseMessage
{
public:
    unsigned int const BLOCK_SIZE = 10;

    void decode(std::vector<uint8_t> const& raw_frame)
    {
        words.clear();
        std::vector<uint8_t> frame;
        for (unsigned int i = 0; i != raw_frame.size(); i += 2)
        {
            frame.insert(frame.begin(), raw_frame[i + 1]);
            frame.insert(frame.begin(), raw_frame[i]);
        }

        uint1024_t data = 0;
        unsigned int offset = BLOCK_SIZE;
        while (offset <= frame.size())
        {
            data = 0;
            for (unsigned int i = 0; i != BLOCK_SIZE; ++i)
                data = (data << 8) | frame[frame.size() - offset + i];
            decode_codeword(data);
            offset += BLOCK_SIZE;
        }
    }

    void encode(std::vector<std::string> const& sequence, nlohmann::json const& json_info)
    {
        std::vector<std::string>::const_iterator it = sequence.begin();
        words.clear();
        nlohmann::json json_info_with_mli = json_info;
        json_info_with_mli["codewords"][0]["data_elements"]["MLI"] = sequence.size() - 1;
        int sequence_position = 0;
        for (; it != sequence.end(); ++it)
        {
            std::vector<std::shared_ptr<IBuilder> >::const_iterator cw_it;
            for (cw_it = codewords_factory.begin(); cw_it != codewords_factory.end(); ++cw_it)
            {
                if ((*cw_it)->name() == *it)
                {
                    std::shared_ptr<ICodeword> cw((*cw_it)->build());
                    cw->encode(json_info_with_mli, sequence_position);
                    words.push_back(cw);
                    break;
                }
            }
            ++sequence_position;
        }
    }

    void encode(nlohmann::json const& json_info)
    {}

    Base_Jx_y_Message()
    {
        codewords_factory = {std::shared_ptr<IBuilder>(new(Builder<Codeword>))... };
    }

    nlohmann::json to_json(bool with_technical=false) const override
    {
        nlohmann::json j;
        j["name"] = name();
        nlohmann::json cw_c = nlohmann::json::array();
        for (std::vector<std::shared_ptr<ICodeword> >::const_iterator it = words.begin();
             it != words.end(); ++it)
        {
            cw_c.push_back((*it)->to_json(with_technical));
        }
        j["codewords"] = cw_c;
        return j;
    }

    void to_bin(std::vector<std::uint8_t>& frame) const override
    {
        for (std::vector<std::shared_ptr<ICodeword> >::const_iterator it = words.begin();
             it != words.end(); ++it)
        {
            uint1024_t cw_data = (*it)->to_bin();
            for (unsigned int i = 0; i != BLOCK_SIZE; i += 2)
            {
                uint8_t first(cw_data & 0xFF);
                cw_data = cw_data >> 8;
                uint8_t second(cw_data & 0xFF);
                cw_data = cw_data >> 8;
                frame.push_back(second);
                frame.push_back(first);
            }
        }
    }

    std::string name() const
    { //return "J3.3";
        std::stringstream ss;
        ss << "J" << label << "." << subLabel;
        return ss.str();
    }

protected:
    std::vector<std::shared_ptr<ICodeword> > words;
    std::vector<std::shared_ptr<IBuilder> > codewords_factory;
private:
    void decode_codeword(uint1024_t const& data)
    {
        std::vector<std::shared_ptr<IBuilder> >::const_iterator it;
        for (it = codewords_factory.begin(); it != codewords_factory.end(); ++it)
        {
            if ((*it)->match(data))
            {
                std::shared_ptr<ICodeword> cw((*it)->build());
                cw->decode(data);
                words.push_back(cw);
                break;
            }
        }
    }
};

std::ostream& operator<<(std::ostream& os, IBaseMessage const& elt);
std::vector<uint8_t>& operator<<(std::vector<uint8_t>& os, IBaseMessage const& elt);
nlohmann::json& operator<<(nlohmann::json& j, IBaseMessage const& elt);

}

#endif // BASEMESSAGE_H
