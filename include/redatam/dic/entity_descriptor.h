#pragma once

#include <string>
#include <array>
#include "../util.h"

namespace Redatam {

class EntityDescriptor {
public:
	std::string name1;
	std::string name2;
	std::string parent;
	std::string description;
	fs::path ptr_path;

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

	// calculated fields
	fs::path real_ptr_path;
	size_t num_instances;

	// looks for ptr file in the provided range of directories
	template<typename First, typename Last>
	bool resolve_ptr_data(First first, Last last) {
		if (ptr_path.empty())
			return false;

		real_ptr_path = ptr_path;
		while (!exists(real_ptr_path) && first != last) {
			real_ptr_path = locate_icase(*first / ptr_path.filename());
			++first;
		}
		if (!exists(real_ptr_path))
			return false;

		std::fstream ptr_file(real_ptr_path.c_str(), std::ios_base::in|std::ios_base::binary);
		if (!ptr_file)
			return false;

		// the ptr file accumulates entity instances by their parent entity's ids. e.g:
		// 3, 5, 9 indicates that there are 9 entities total:
		// 3 children of parent 1, 2 children of parent 2 and 4 children of parent 3
		ptr_file.seekg(-sizeof(uint32_t), std::ios_base::end);
		num_instances = fread_uint32_t(ptr_file);
		return true;
	}

	static EntityDescriptor fread(std::istream& stream, bool is_root);
};
std::ostream& operator<<(std::ostream& stream, const EntityDescriptor& d);

} // namespace Redatam
