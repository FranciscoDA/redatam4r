#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "primitives.h"
#include "util.h"
#include "dic/dictionary_descriptor.h"
#include "dic/entity_descriptor.h"
#include "dic/variable_descriptor.h"
#include "redatam4r.h"
#include "protected_sexp.h"
#include <R.h>
#include <Rdefines.h>

template<typename T>
SEXP serialize_to_R(const T& obj) {
	std::stringstream ss;
	ss << obj << '\n';
	return mkString(ss.str().c_str());
}

extern "C" {
SEXP read_redatam(SEXP dic_path_in) {
	UnixPath dic_path(CHAR(asChar(dic_path_in)));
	std::fstream dic_file(dic_path.as_string(), std::ios_base::in|std::ios_base::binary);

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);

	ProtectedSEXP description_rsymbol = mkString("description");
	ProtectedSEXP levelkeys_rsymbol = mkString("levelkeys");

	ProtectedSEXP entity_class_rvector = allocVector(STRSXP, 2);
	SET_STRING_ELT(entity_class_rvector, 0, mkChar("redatam.entity"));
	SET_STRING_ELT(entity_class_rvector, 1, mkChar("data.frame"));

	ProtectedSEXP variable_int_class_rvector = mkString("redatam.variable.int");
	ProtectedSEXP variable_lng_class_rvector = mkString("redatam.variable.lng");
	ProtectedSEXP variable_chr_class_rvector = mkString("redatam.variable.chr");
	ProtectedSEXP variable_bin_class_rvector = mkString("redatam.variable.bin");
	ProtectedSEXP variable_dbl_class_rvector = mkString("redatam.variable.dbl");
	ProtectedSEXP variable_pck_class_rvector = mkString("redatam.variable.pck");

	ProtectedSEXP instance_len_rsymbol = mkString("instance.len");
	ProtectedSEXP instance_num_rsymbol = mkString("instance.num");
	ProtectedSEXP data_path_rsymbol = mkString("data.path");

	ProtectedSEXP ans = allocVector(VECSXP, root_entity.num_entities-1);
	ProtectedSEXP ans_names = allocVector(STRSXP, root_entity.num_entities-1);
	setAttrib(ans, description_rsymbol, serialize_to_R(dic));
	classgets(ans, mkString("redatam.database"));

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

				ProtectedSEXP current_entity = allocVector(VECSXP, entity.num_vars+1);
				ProtectedSEXP current_entity_names = allocVector(STRSXP, entity.num_vars+1);
				ProtectedSEXP column0 = allocVector(INTSXP, num_instances);

				uint32_t prev_n;
				uint32_t next_n;
				int parent_id = 0;
				prev_n = fread_uint32_t(ptr_file); // always reads 0 at offset 0
				while (next_n = fread_uint32_t(ptr_file), ptr_file) {
					++parent_id;
					for (uint32_t k = prev_n; k < next_n; ++k) {
						SET_INTEGER_ELT(column0, k, parent_id);
					}
					prev_n = next_n;
				}
				setAttrib(column0, description_rsymbol, mkString(("Parent instance row in " + entity.parent + "(surrogate attribute)\n").c_str()));
				SET_VECTOR_ELT(current_entity, 0, column0);
				SET_STRING_ELT(current_entity_names, 0, mkChar((entity.parent + "_id").c_str()));

				// compact row names representation (see .set_row_names and .row_names_info)
				ProtectedSEXP rownames = allocVector(INTSXP, 2);
				SET_INTEGER_ELT(rownames, 0, R_NaInt);
				SET_INTEGER_ELT(rownames, 1, -num_instances);
				setAttrib(current_entity, R_RowNamesSymbol, rownames);

				classgets(current_entity, entity_class_rvector);
				namesgets(current_entity, current_entity_names);

				setAttrib(current_entity, description_rsymbol, serialize_to_R(entity));
				SET_VECTOR_ELT(ans, entity_count-1, current_entity);

				for (size_t i = 0; i < entity.num_vars; ++i) {
					ProtectedSEXP column = allocVector(INTSXP, 0);
					VariableDescriptor var = VariableDescriptor::fread(dic_file);
					if (var.declaration) {
						auto rbf_real_basename = locate_icase(dic_path.dir().as_string(), var.declaration->rbf_path.basename());
						if (!rbf_real_basename.empty()) {
							auto rbf_real_path = dic_path.dir() + rbf_real_basename;
							//std::fstream rbf_file(rbf_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
							setAttrib(column, instance_len_rsymbol, ScalarInteger(var.declaration->size));
							setAttrib(column, instance_num_rsymbol, ScalarInteger(num_instances));
							setAttrib(column, data_path_rsymbol, mkString(rbf_real_path.as_string().c_str()));

							switch (var.declaration->type) {
								case VariableDescriptor::Declaration::Type::DBL:
									classgets(column, variable_dbl_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::CHR:
									classgets(column, variable_chr_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::INT:
									classgets(column, variable_int_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::LNG:
									classgets(column, variable_lng_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::BIN:
									classgets(column, variable_bin_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::PCK: {
									classgets(column, variable_pck_class_rvector);
									break;
								}
								default:
									break;
							}
						}
					}
					SET_STRING_ELT(current_entity_names, i+1, mkChar(var.name.c_str()));
					SET_VECTOR_ELT(current_entity, i+1, column);

					if (var.labels.size() > 0 and (
							var.declaration->type == VariableDescriptor::Declaration::Type::LNG or
							var.declaration->type == VariableDescriptor::Declaration::Type::INT or
							var.declaration->type == VariableDescriptor::Declaration::Type::PCK or
							var.declaration->type == VariableDescriptor::Declaration::Type::BIN)
					) {
						std::vector<std::pair<int, std::string>> levels = var.labels;
						if (var.descriptor.missing)
							levels.emplace_back(*var.descriptor.missing, "<MISS>");
						if (var.descriptor.not_applicable)
							levels.emplace_back(*var.descriptor.not_applicable, "<NOAP>");
						std::sort(levels.begin(), levels.end());
						ProtectedSEXP levels_rvector = allocVector(STRSXP, levels.size());
						ProtectedSEXP levelkeys_rvector = allocVector(INTSXP, levels.size());
						for (int i = 0; i < levels.size(); ++i) {
							SET_STRING_ELT(levels_rvector, i, mkChar(levels[i].second.c_str()));
							SET_INTEGER_ELT(levelkeys_rvector, i, levels[i].first);
						}
						setAttrib(column, R_LevelsSymbol, levels_rvector);
						setAttrib(column, levelkeys_rsymbol, levelkeys_rvector);
					}
					setAttrib(column, description_rsymbol, serialize_to_R(var));
				}
			}
		}
	}

	namesgets(ans, ans_names);
	return ans;
}
} // extern "C"
