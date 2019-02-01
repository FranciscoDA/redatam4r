#pragma once

#include <string>
#include <array>

namespace Redatam {

class DictionaryDescriptor {
public:
	uint32_t unknown1;
	std::string name;
	std::array<char, 8> creation_date;
	std::array<char, 8> modification_date;
	std::string root_dir;
	std::string unknown2;

	static DictionaryDescriptor fread(std::istream& stream);
};
std::ostream& operator<<(std::ostream& stream, const DictionaryDescriptor& d);

}
