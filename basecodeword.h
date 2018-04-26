#ifndef BASE_CODEWORD_H_233920180208
#define BASE_CODEWORD_H_233920180208

#include <exception>
#include <math.h>
#include <memory>
#include <sstream>
#include <vector>

#include "dataelements.h"

#include "json.hpp"

namespace L16 __attribute__((visibility ("hidden"))) {

char codeword_format(unsigned int v);

class ICodeword
{
public:
	friend std::ostream& operator<<(std::ostream& os, L16::ICodeword const& elt);
    std::vector<std::shared_ptr<IDataElement> > fields;
    virtual nlohmann::json to_json(bool with_technical=false) const = 0;
    virtual uint1024_t to_bin() const = 0;
    virtual void decode(uint1024_t data) = 0;
    virtual void encode(nlohmann::json const& json_info, int pos) = 0;
    virtual std::string codeword_name() const = 0;
};

template <unsigned int wf, class ...Fields> struct Codeword: public ICodeword
{
    Codeword()
    {
		fields = {std::shared_ptr<IDataElement>(new T_1550_1_WORD_FORMAT<0, wf>()), std::shared_ptr<IDataElement>(new(Fields))... };
    }
    nlohmann::json to_json(bool with_technical=false) const override
    {
        nlohmann::json j;
        nlohmann::json data_element;
        for (std::vector<std::shared_ptr<IDataElement> >::const_iterator i = fields.begin(); i != fields.end(); ++i)
        {

            if (!(*i)->is_technical() || with_technical)
                data_element[(*i)->name()] = (*i)->value();
        }
        j["name"] = codeword_name();
        j["data_elements"] = data_element;
        return j;
    }

    uint1024_t to_bin() const override
    {
        uint1024_t data = 0;
        for (std::vector<std::shared_ptr<IDataElement> >::const_iterator i = fields.begin(); i != fields.end(); ++i)
        {
            data << **i;
            //std::cout << std::hex << data << " " << (*i)->name() << " " << (*i)->value() << std::endl;
        }
        return data;
    }

    virtual void decode(uint1024_t data)
    {
        check(data);
        for (std::vector<std::shared_ptr<IDataElement> >::iterator i = fields.begin(); i != fields.end(); ++i)
        {
            data >> **i;
        }
    }

    void encode(nlohmann::json const& json_info, int pos)
    {
        for (std::vector<std::shared_ptr<IDataElement> >::iterator i = fields.begin(); i != fields.end(); ++i)
        {
            json_info["codewords"][pos]["data_elements"] >> **i;
            if (!(*i)->is_valid())
            {
                json_info >> **i;
            }
        }
    }

    virtual void check(uint1024_t const& data) const = 0;
};

class IllegalCodeWord: public std::exception
{
public:
    IllegalCodeWord(std::string const& txt) throw(): text(txt)
    {}

    virtual const char* what() const throw()
    {
        return text.c_str();
    }

    virtual ~IllegalCodeWord() throw()
    {}
private:
    std::string text;
};


template <unsigned int label, unsigned int subLabel, class ... Fields> struct Initial: public Codeword<0, T_270_4_LABEL_J_SERIES<2, label>,
		T_271_5_SUBLABEL_J_SERIES<7, subLabel>,
		T_800_1_MESSAGE_LENGTH_INDICATOR<10>, Fields...>
{
    void check(uint1024_t const& data) const override
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        if (data_wf == 0)
            return;
        unsigned int data_cont_label = static_cast<unsigned int>((data & 0b111100) >> 2);
        std::stringstream ss;
        ss << "Expected Initial word";
        ss << ". But found " << codeword_format(data_wf);
        if (data_wf == 1)
            ss << data_cont_label;
        throw IllegalCodeWord(ss.str());
    }

    static bool match(uint1024_t const& data)
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        return (data_wf == 0);
    }

    static std::string name()
    {
        return "I";
    }

    std::string codeword_name() const override
    {
        return Initial::name();
    }
};

template <unsigned int label, class ... Fields> struct Extension: public Codeword<2, Fields...>
{
    void check(uint1024_t const& data) const override
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        if (data_wf == 0b10)
            return;
        unsigned int data_cont_label = static_cast<unsigned int>((data & 0b111100) >> 2);
        std::stringstream ss;
        ss << "Expected Extension word";
        ss << ". But found " << codeword_format(data_wf);
        if (data_wf == 1)
            ss << data_cont_label;
        throw IllegalCodeWord(ss.str());
    }

    static bool match(uint1024_t const& data)
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        return (data_wf == 0b10);
    }

    static std::string name() {
        std::stringstream ss;
        ss << "E" << label;
        return ss.str();
    }

    std::string codeword_name() const override
    {
        return Extension::name();
    }
};

template<unsigned int cont_label, class ...Fields> struct Continuation: public Codeword<1,
		T_1551_1_CONTINUATION_WORD_LABEL<2>,
        Fields...>
{
    void check(uint1024_t const& data) const override
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        unsigned int data_cont_label = static_cast<unsigned int>((data & 0b111100) >> 2);
        if ((data_wf == 0b01) && (data_cont_label == cont_label))
            return;
        std::stringstream ss;
        ss << "Expected Continuation Word C" << cont_label;
        ss << ". But found " << codeword_format(data_wf);
        if (data_wf == 1)
            ss << data_cont_label;
        throw IllegalCodeWord(ss.str());
    }

    static bool match(uint1024_t const& data)
    {
        unsigned int data_wf = static_cast<unsigned int>(data & 0b0011);
        unsigned int data_cont_label = static_cast<unsigned int>((data & 0b111100) >> 2);
        return ((data_wf == 0b01) && (data_cont_label == cont_label));

    }

    static unsigned int get_continuation_word_label(uint1024_t const& data){
        return static_cast<unsigned int>((data & 0b111100) >> 2);
    }

    static std::string name() {
        std::stringstream ss;
        ss << "C" << cont_label;
        return ss.str();
    }

    std::string codeword_name() const override
    {
        return Continuation::name();
    }
};
}


std::ostream& operator<<(std::ostream& os, L16::ICodeword const& elt);

#endif // BASE_CODEWORD_H_233920180208
