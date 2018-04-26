
#ifndef DATA_ELEMENTS_H
#define DATA_ELEMENTS_H

#include <iostream>

#include "basedataelement.h"


namespace L16 __attribute__((visibility ("hidden"))) {


template <unsigned int position, unsigned int ns=-1> struct T_1550_1_WORD_FORMAT : public DataElement<756, 57, position, 2, ns, IDataElement::Tactical>
{
    inline std::string name() const override { return "WORD FORMAT"; }
};

template <unsigned int position, unsigned int ns=-1> struct T_270_4_LABEL_J_SERIES : public DataElement<756, 57, position, 5, ns, IDataElement::Tactical>
{
    inline std::string name() const override { return "LABEL, J-SERIES"; }
};

template <unsigned int position, unsigned int ns=-1> struct T_271_5_SUBLABEL_J_SERIES : public DataElement<756, 57, position, 3, ns, IDataElement::Tactical>
{
    inline std::string name() const override { return "SUBLABEL, J-SERIES"; }
};

template <unsigned int position, unsigned int ns=-1> struct T_800_1_MESSAGE_LENGTH_INDICATOR : public DataElement<756, 57, position, 3, ns, IDataElement::Tactical>
{
    inline std::string name() const override { return "MESSAGE LENGTH INDICATOR"; }
};


	}
	#endif // DATA_ELEMENTS_H
	
