#include "l16_decoder.h"

#include "l16_ed7_messages.h"


char* L16_Decoder::decode_to_json(int length, char* data)
{
	std::vector<uint8_t> vec(data, data + length);
	std::string s = decode_to_json(vec);
	char* c = new char[s.length()];
	strcpy(c, s.c_str());
	//return decode_to_json(vec);
	return c;
}

std::string L16_Decoder::decode_to_json(std::vector<uint8_t> const& data)
{
	L16::J3_3 j;
	j.decode(data);
	return j.to_json().dump(2);
}

void L16_Decoder::encode_from_json(std::vector<std::string> const& sequence, std::string const& json_info, std::vector<uint8_t>& data)
{
	nlohmann::json j = nlohmann::json::parse(json_info);
	L16::J3_3 msg;
	msg.encode(sequence, j);
	msg.to_bin(data);
}
