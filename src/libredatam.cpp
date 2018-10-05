#include <string>
#include <iostream>
#include <fstream>
#include "primitives.h"
#include "util.h"
#include "dic/dictionary_descriptor.h"
#include "dic/entity_descriptor.h"
#include "dic/variable_descriptor.h"
#include "libredatam.h"
#include <R.h>

void dataframe_init(SEXP& s, size_t width, size_t height) {
	SEXP cls, rownames, colnames;
	PROTECT(s = allocVector(VECSXP, width));
	// set class
	PROTECT(cls = allocVector(STRSXP, 1));
	SET_STRING_ELT(cls, 0, mkChar("data.frame"));
	classgets(s, cls);
	// set row names
	PROTECT(rownames = allocVector(STRSXP, height));
	for (size_t i = 0; i < height; ++i)
		SET_STRING_ELT(rownames, i, mkChar(std::to_string(i+1).c_str()));
	setAttrib(s, R_RowNamesSymbol, rownames);
	// init col names vector
	PROTECT(colnames = allocVector(STRSXP, width));
	namesgets(cls, colnames);
}
void dataframe_initcol(SEXP& s, size_t colidx, size_t height, int type, const char* colname) {
	SEXP row;
	PROTECT(row = allocVector(type, height));
	SET_VECTOR_ELT(s, colidx, row);
	SET_STRING_ELT(GetColNames(s), colidx, mkChar(colname));
}

SEXP read_redatam(SEXP dic_path_in) {
	UnixPath dic_path(CHAR(asChar(dic_path_in)));
	std::fstream dic_file(dic_path.as_string(), std::ios_base::in|std::ios_base::binary);

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);

	SEXP ans, current_entity, current_entity_symbol, current_variable;

	// allocate the root list (no elements)
	PROTECT(ans = allocVector(STRSXP, 0));

	// allocate the attribute for the root entity
	PROTECT(current_entity_symbol = allocVector(STRSXP, 1));
	SET_STRING_ELT(current_entity_symbol, 0, mkChar(root_entity.name1.c_str()));
	PROTECT(current_entity = allocVector(VECSXP, root_entity.num_vars));
	setAttrib(ans, current_entity_symbol, current_entity);

	for (int entity_count = 1; entity_count < root_entity.num_entities; ++entity_count) {
		auto entity = EntityDescriptor::fread(dic_file, false);
		PROTECT(current_entity_symbol = allocVector(STRSXP, 1));
		SET_STRING_ELT(current_entity_symbol, 0, mkChar(entity.name1.c_str()));
		// add one extra vector for parent id
		//PROTECT(current_entity = allocVector(VECSXP, entity.num_vars+1));

		uint32_t num_instances = 0;
		if (!entity.ptr_path.empty()) {
			auto ptr_real_basename = locate_icase(dic_path.dir().as_string(), entity.ptr_path.basename());
			if (!ptr_real_basename.empty()) {
				auto ptr_real_path = dic_path.dir() + ptr_real_basename;
				std::fstream ptr_file(ptr_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
				ptr_file.seekg(sizeof(uint32_t), std::ios_base::end);
				num_instances = fread_uint32_t(ptr_file);
				PROTECT(current_variable = allocVector(INTSXP, num_instances));
				ptr_file.seekg(0, std::ios_base::beg);
				dataframe_init(current_entity, 1, num_instances);
				dataframe_initcol(current_entity, 0, num_instances, INTSXP, "parent_id");
				uint32_t prev_n;
				uint32_t next_n;
				int parent_id = 0;
				fread_uint32_t(prev_n, ptr_file);
				while (fread_uint32_t(next_n, ptr_file)) {
					++parent_id;
					for (size_t k = prev_n; k < next_n; ++k) {
						SET_INTEGER_ELT(VECTOR_ELT(current_entity, 0), k, parent_id);
					}
					prev_n = next_n;
				}
			}
		}
		for (size_t i = 0; i < entity.num_vars; ++i) {
			VariableDescriptor var = VariableDescriptor::fread(dic_file);
			// do sth with var
			if (var.declaration) {
				auto rbf_real_basename = locate_icase(dic_path.dir().as_string(), var.declaration->rbf_path.basename());
				if (!rbf_real_basename.empty()) {
					auto rbf_real_path = dic_path.dir() + rbf_real_basename;
					std::fstream rbf_file(rbf_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
					rbf_file.seekg(0, std::ios_base::end);
					size_t rbf_file_sz = rbf_file.tellg();
					// do sth?
				}
			}
		}
	}
	return ans;
}
