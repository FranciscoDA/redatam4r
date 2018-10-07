#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "primitives.h"
#include "util.h"
#include "dic/dictionary_descriptor.h"
#include "dic/entity_descriptor.h"
#include "dic/variable_descriptor.h"
#include "redatam4r.h"
#include <R.h>

extern "C" {
SEXP read_redatam(SEXP dic_path_in) {
	UnixPath dic_path(CHAR(asChar(dic_path_in)));
	std::fstream dic_file(dic_path.as_string(), std::ios_base::in|std::ios_base::binary);

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);

	SEXP ans, ans_names;

	// allocate the root object
	PROTECT(ans = allocVector(VECSXP, root_entity.num_entities-1));
	PROTECT(ans_names = allocVector(STRSXP, root_entity.num_entities-1));
	classgets(ans, mkString("redatam.database"));

	SEXP entity_description_rsymbol, entity_documentation_rsymbol, entity_class_rvector;
	PROTECT(entity_description_rsymbol = mkString("description"));
	PROTECT(entity_documentation_rsymbol = mkString("documentation"));

	PROTECT(entity_class_rvector = allocVector(STRSXP, 2));
	SET_STRING_ELT(entity_class_rvector, 0, mkChar("redatam.entity"));
	SET_STRING_ELT(entity_class_rvector, 1, mkChar("data.frame"));

	std::string last_entity_name = root_entity.name1;
	for (int entity_count = 1; entity_count < root_entity.num_entities; ++entity_count) {
		auto entity = EntityDescriptor::fread(dic_file, false);
		SET_STRING_ELT(ans_names, entity_count-1, mkChar(entity.name1.c_str()));

		uint32_t num_instances = 0;
		if (!entity.ptr_path.empty()) {
			auto ptr_real_basename = locate_icase(dic_path.dir().as_string(), entity.ptr_path.basename());
			if (!ptr_real_basename.empty()) {
				auto ptr_real_path = dic_path.dir() + ptr_real_basename;
				std::fstream ptr_file(ptr_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
				ptr_file.seekg(-sizeof(uint32_t), std::ios_base::end);
				num_instances = fread_uint32_t(ptr_file);
				ptr_file.seekg(0, std::ios_base::beg);

				SEXP current_entity, names, rownames;
				PROTECT(current_entity = allocVector(VECSXP, 1));
				PROTECT(names = allocVector(STRSXP, 1));

				SEXP column;
				PROTECT(column = allocVector(INTSXP, num_instances));

				uint32_t prev_n;
				uint32_t next_n;
				int parent_id = 0;
				fread_uint32_t(prev_n, ptr_file); // always reads 0 at offset 0
				while (fread_uint32_t(next_n, ptr_file)) {
					++parent_id;
					for (uint32_t k = prev_n; k < next_n; ++k) {
						SET_INTEGER_ELT(column, k, parent_id);
					}
					prev_n = next_n;
				}
				SET_VECTOR_ELT(current_entity, 0, column);
				SET_STRING_ELT(names, 0, mkChar((last_entity_name + "_id").c_str()));

				PROTECT(rownames = allocVector(INTSXP, num_instances));
				for (int z = 0; z < num_instances; ++z)
					SET_INTEGER_ELT(rownames, z, z+1);

				classgets(current_entity, entity_class_rvector);
				namesgets(current_entity, names);
				setAttrib(current_entity, R_RowNamesSymbol, rownames);
				setAttrib(current_entity, entity_description_rsymbol, mkString(entity.description.c_str()));
				setAttrib(current_entity, entity_documentation_rsymbol, mkString(entity.documentation.c_str()));
				SET_VECTOR_ELT(ans, entity_count-1, current_entity);

				UNPROTECT(4); // names, rownames, column, current_entity
				for (size_t i = 0; i < entity.num_vars; ++i) {
					VariableDescriptor var = VariableDescriptor::fread(dic_file);
					if (var.declaration) {
						auto rbf_real_basename = locate_icase(dic_path.dir().as_string(), var.declaration->rbf_path.basename());
						if (!rbf_real_basename.empty()) {
							auto rbf_real_path = dic_path.dir() + rbf_real_basename;
							std::fstream rbf_file(rbf_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
							rbf_file.seekg(0, std::ios_base::end);
							size_t rbf_file_sz = rbf_file.tellg();
						}
					}
				}
			}
		}
		last_entity_name = entity.name1;
	}

	namesgets(ans, ans_names);
	UNPROTECT(5); // ans, ans_names, entity_description_rsymbol, entity_documentation_rsymbol, entity_class_vector

	return ans;
}
} // extern "C"
