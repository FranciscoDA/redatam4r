#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "primitives.h"
#include "dic/dictionary_descriptor.h"
#include "dic/entity_descriptor.h"
#include "dic/variable_descriptor.h"
#include "redatam_exception.h"

namespace Redatam {

template<typename F, typename G, typename H>
void read_dic(const fs::path& dic_path, F&& dictionary_cb, G&& entity_cb, H&& variable_cb) {
	std::fstream dic_file(dic_path, std::ios_base::in|std::ios_base::binary);

	if (!dic_file)
		throw Exception(std::string("Could not open dictionary file for reading: ") + std::string(dic_path));

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);
	dictionary_cb(dic, root_entity);

	std::vector<fs::path> search_paths;
	search_paths.push_back(dic_path.parent_path());

	for (size_t entity_count = 1; entity_count < root_entity.num_entities; ++entity_count) {
		EntityDescriptor entity = EntityDescriptor::fread(dic_file, false);
		if (!entity.resolve_ptr_data(begin(search_paths), end(search_paths)))
			throw Exception(std::string("Could not read ptr data file: ") + std::string(entity.ptr_path));

		entity_cb(entity, entity_count);

		for (size_t var_id = 0; var_id < entity.num_vars; ++var_id) {
			VariableDescriptor variable = VariableDescriptor::fread(dic_file);
			if (!variable.resolve_rbf_data(begin(search_paths), end(search_paths)))
				throw Exception(std::string("Could not read rbf data file: ") + std::string(variable.declaration->rbf_path));

			variable_cb(variable, entity, var_id);
		}
	}
}

} // namespace Redatam
