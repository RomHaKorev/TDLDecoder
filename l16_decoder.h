#ifndef L16_DECODER_H
#define L16_DECODER_H

#include <memory>
#include <vector>
#include <string>
#include <math.h>

#include "json.hpp"

#include <QtCore>

namespace L16_Decoder
{
	//extern "C" __attribute__((visibility("default"))) int foobar(int length, char* data);
	extern "C" __attribute__((visibility("default"))) char* decode_to_json(int length, char* data);
	//extern "C" __attribute__((visibility("default"))) std::string decode_to_json(std::vector<uint8_t> const& data);
	__attribute__((visibility("hidden"))) std::string decode_to_json(std::vector<uint8_t> const& data);
	extern "C" __attribute__((visibility("default"))) void encode_from_json(std::vector<std::string> const& sequence, std::string const& json_info, std::vector<uint8_t>& data);
}

#endif // L16_DECODER_H
