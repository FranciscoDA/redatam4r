#ifndef _REDATAM4R_ENTITY_DESCRIPTOR_H_
#define _REDATAM4R_ENTITY_DESCRIPTOR_H_

#include <string>
#include <array>
#include "../util.h"

class EntityDescriptor {
public:
	std::string name1;
	std::string name2;
	std::string parent;
	std::string description;
	DOSPath ptr_path;

	uint16_t unknown1;

	std::string id_varname;
	std::string name_varname;
	std::string documentation;

	uint16_t num_entities;
	std::array<unsigned char, 3> unknown2;

	uint32_t num_vars;

	uint16_t unknown3;
	uint16_t is_selectable;
	std::array<unsigned char, 16> unknown6;

	std::string label;

	uint16_t is_sensible;
	uint16_t sensible_threshold;

	std::string weight_varname;

	static EntityDescriptor fread(std::istream& stream, bool is_root);
};
std::ostream& operator<<(std::ostream& stream, const EntityDescriptor& d);

#endif
