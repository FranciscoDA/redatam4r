#ifndef _REDATAM4R_VARIABLE_DESCRIPTOR_H_
#define _REDATAM4R_VARIABLE_DESCRIPTOR_H_

#include <fstream>
#include <string>
#include <array>
#include <optional>

class VariableDescriptor {
public:
	class Declaration {
	public:
		enum class Type {
			BIN, CHR, DBL, INT, LNG, PCK
		};
		static std::optional<Declaration> fromDeclarationString(const std::string& declstr);
		Type type;
		DOSPath rbf_path;
		size_t size;
	};
	class Descriptor {
	public:
		std::string alias;
		size_t decimals = 0;
		std::string group;
		std::optional<int> missing;
		std::optional<int> not_applicable;
	};
	std::string name;
	std::optional<Declaration> declaration;
	std::string filter;
	std::string range;
	std::string datatype;
	std::vector<std::pair<int, std::string>> labels;
	//std::string labels_str;
	std::string description;
	Descriptor descriptor;

	uint16_t unknown1;
	std::string documentation;
	uint16_t id;
	std::array<unsigned char, 16> unknown;

	static VariableDescriptor fread(std::istream& stream);
};
std::ostream& operator<<(std::ostream& stream, const VariableDescriptor& d);
std::ostream& operator<<(std::ostream& stream, const VariableDescriptor::Declaration& d);

#endif

