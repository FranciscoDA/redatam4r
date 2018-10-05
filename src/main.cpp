#include <string>
#include <iostream>
#include <fstream>
#include "primitives.h"
#include "util.h"
#include "dic/dictionary_descriptor.h"
#include "dic/entity_descriptor.h"
#include "dic/variable_descriptor.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "No dic file!" << std::endl;
		return 1;
	}
	UnixPath dic_path(argv[1]);
	std::fstream dic_file(dic_path.as_string(), std::ios_base::in|std::ios_base::binary);
	std::cout << "Reading From: " << dic_path << std::endl;
	if (!dic_file.good()) {
		std::cout << "Cannot read dic file!" << std::endl;
		return 1;
	}

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);
	std::cout
		<< dic << std::endl
		<< root_entity << std::endl;

	for (int entity_count = 1; entity_count < root_entity.num_entities; ++entity_count) {
		auto entity = EntityDescriptor::fread(dic_file, false);
		size_t entity_pos = dic_file.tellg();
		std::cout
			<< "===ENTITY (" << entity_count << ") @ " << entity_pos << "===\n"
			<< entity << std::endl;
		if (!entity.ptr_path.empty()) {
			auto ptr_real_basename = locate_icase(dic_path.dir().as_string(), entity.ptr_path.basename());
			if (!ptr_real_basename.empty()) {
				auto ptr_real_path = dic_path.dir() + ptr_real_basename;
				uint32_t max = 0;
				std::fstream ptr_file(ptr_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
				for(uint32_t i = 0; fread_uint32_t(i, ptr_file);) {
					max = i;
				}
				std::cout << "Number of entities: " << max << std::endl;
			}
		}
		for (size_t i = 0; i < entity.num_vars; ++i) {
			VariableDescriptor var = VariableDescriptor::fread(dic_file);
			std::cout
				<< "---Variable(" << i << ")---\n"
				<< var << std::endl;
			if (var.declaration) {
				auto rbf_real_basename = locate_icase(dic_path.dir().as_string(), var.declaration->rbf_path.basename());
				if (!rbf_real_basename.empty()) {
					auto rbf_real_path = dic_path.dir() + rbf_real_basename;
					std::fstream rbf_file(rbf_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
					rbf_file.seekg(0, std::ios_base::end);
					size_t rbf_file_sz = rbf_file.tellg();
					std::cout << "Number of instances: " << (rbf_file_sz / var.declaration->size) << std::endl;
				}
			}
		}
	}
	std::cout << "Final position in file: " << dic_file.tellg() << std::endl;
}
