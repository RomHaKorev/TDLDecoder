#include "basecodeword.h"

namespace L16 __attribute__((visibility ("hidden"))) {

char codeword_format(unsigned int v)
{
	switch(v)
	{
	case 0:
		return 'I';
	case 1:
		return 'C';
	case 2:
		return 'E';
	default:
		return 'X';
	}
}

}

std::ostream& operator<<(std::ostream& os, L16::ICodeword const& elt)
{
	for (std::vector<std::shared_ptr<L16::IDataElement> >::const_iterator i = elt.fields.begin(); i != elt.fields.end(); ++i)
		os << (**i) << std::endl;
	return os;
}
