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

class RAIISEXP {
public:
	RAIISEXP() = delete;
	RAIISEXP(SEXP v) : sexp(v) {
		PROTECT(sexp);
	}
	~RAIISEXP() {
		UNPROTECT(1);
	}
	operator SEXP() {
		return sexp;
	}
private:
	SEXP sexp;
};

extern "C" {
SEXP binpck_get(SEXP binpck, SEXP indices) {
	const int instance_len = INTEGER_ELT(getAttrib(binpck, mkString("instance.len")), 0);
	//const int instance_num = INTEGER_ELT(getAttrib(binpck, mkString("instance.num")), 0);
	int ans_len = length(indices);
	RAIISEXP ans = allocVector(INTSXP, ans_len);
	
	const size_t instances_per_element = sizeof(uint32_t) * 8 / instance_len;
	const size_t mask = (1 << instance_len) - 1;
	for (int x = 0; x < ans_len; ++x) {
		int instance_index = INTEGER_ELT(indices, x);
		uint32_t element = INTEGER_ELT(binpck, instance_index / instances_per_element);
		SET_INTEGER_ELT(ans, x, element & (mask << (instance_index % instances_per_element)));
	}
	return ans;
}
SEXP read_redatam(SEXP dic_path_in) {
	UnixPath dic_path(CHAR(asChar(dic_path_in)));
	std::fstream dic_file(dic_path.as_string(), std::ios_base::in|std::ios_base::binary);

	auto dic = DictionaryDescriptor::fread(dic_file);
	auto root_entity = EntityDescriptor::fread(dic_file, true);

	RAIISEXP ans = allocVector(VECSXP, root_entity.num_entities-1);
	classgets(ans, mkString("redatam.database"));

	RAIISEXP ans_names = allocVector(STRSXP, root_entity.num_entities-1);

	RAIISEXP description_rsymbol = mkString("description");
	RAIISEXP documentation_rsymbol = mkString("documentation");
	RAIISEXP missing_rsymbol = mkString("missing.value");
	RAIISEXP notapplicable_rsymbol = mkString("notapplicable.value");

	RAIISEXP entity_class_rvector = allocVector(STRSXP, 2);
	SET_STRING_ELT(entity_class_rvector, 0, mkChar("redatam.entity"));
	SET_STRING_ELT(entity_class_rvector, 1, mkChar("data.frame"));

	RAIISEXP variable_int_class_rvector = mkString("redatam.variable.int");
	RAIISEXP variable_lng_class_rvector = mkString("redatam.variable.lng");
	RAIISEXP variable_chr_class_rvector = mkString("redatam.variable.chr");
	RAIISEXP variable_bin_class_rvector = mkString("redatam.variable.bin");
	RAIISEXP variable_dbl_class_rvector = mkString("redatam.variable.dbl");
	RAIISEXP variable_pck_class_rvector = mkString("redatam.variable.pck");

	// special attributes for BIN and PCK
	RAIISEXP binpck_instance_len = mkString("instance.len");
	RAIISEXP binpck_instance_num = mkString("instance.num");

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

				SEXP current_entity, names, rownames, column;
				PROTECT(current_entity = allocVector(VECSXP, entity.num_vars+1));
				PROTECT(names = allocVector(STRSXP, entity.num_vars+1));
				PROTECT(column = allocVector(INTSXP, num_instances));

				uint32_t prev_n;
				uint32_t next_n;
				int parent_id = 0;
				prev_n = fread_uint32_t(ptr_file); // always reads 0 at offset 0
				while (next_n = fread_uint32_t(ptr_file), ptr_file) {
					++parent_id;
					for (uint32_t k = prev_n; k < next_n; ++k) {
						SET_INTEGER_ELT(column, k, parent_id);
					}
					prev_n = next_n;
				}
				classgets(column, variable_lng_class_rvector);
				setAttrib(column, description_rsymbol, mkString(("Parent instance row in " + entity.parent).c_str()));
				SET_VECTOR_ELT(current_entity, 0, column);
				SET_STRING_ELT(names, 0, mkChar((entity.parent + "_id").c_str()));

				// compact row names representation
				// see .set_row_names and .row_names_info
				PROTECT(rownames = allocVector(INTSXP, 2));
				SET_INTEGER_ELT(rownames, 0, R_NaInt);
				SET_INTEGER_ELT(rownames, 1, -num_instances);

				classgets(current_entity, entity_class_rvector);
				namesgets(current_entity, names);
				setAttrib(current_entity, R_RowNamesSymbol, rownames);
				setAttrib(current_entity, description_rsymbol, mkString(entity.description.c_str()));
				setAttrib(current_entity, documentation_rsymbol, mkString(entity.documentation.c_str()));
				SET_VECTOR_ELT(ans, entity_count-1, current_entity);

				UNPROTECT(4); // rownames, column, names, current_entity
				for (size_t i = 0; i < entity.num_vars; ++i) {
					VariableDescriptor var = VariableDescriptor::fread(dic_file);
					if (var.declaration) {
						auto rbf_real_basename = locate_icase(dic_path.dir().as_string(), var.declaration->rbf_path.basename());
						if (!rbf_real_basename.empty()) {
							auto rbf_real_path = dic_path.dir() + rbf_real_basename;
							std::fstream rbf_file(rbf_real_path.as_string(), std::ios_base::in|std::ios_base::binary);
							switch (var.declaration->type) {
								case VariableDescriptor::Declaration::Type::DBL:
									PROTECT(column = allocVector(REALSXP, num_instances));
									for (size_t j = 0; j < num_instances; ++j) {
										double elt = fread_double(rbf_file);
										if (rbf_file)
											SET_REAL_ELT(column, j, elt);
										else
											break;
									}
									classgets(column, variable_dbl_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::CHR:
									PROTECT(column = allocVector(STRSXP, num_instances));
									for (size_t j = 0; j < num_instances; ++j) {
										std::string elt = fread_fixed_string(rbf_file, var.declaration->size);
										if (rbf_file)
											SET_STRING_ELT(column, j, mkChar(elt.c_str()));
										else
											break;
									}
									classgets(column, variable_chr_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::INT:
									PROTECT(column = allocVector(INTSXP, num_instances));
									for (size_t j = 0; j < num_instances; ++j) {
										uint16_t elt = fread_uint16_t(rbf_file);
										if (rbf_file)
											if (elt == var.descriptor.missing or elt == var.descriptor.not_applicable)
												SET_INTEGER_ELT(column, j, NA_INTEGER);
											else
												SET_INTEGER_ELT(column, j, elt);
										else
											break;
									}
									classgets(column, variable_int_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::LNG:
									PROTECT(column = allocVector(INTSXP, num_instances));
									for (size_t j = 0; j < num_instances; ++j) {
										uint32_t elt = fread_uint32_t(rbf_file);
										if (rbf_file)
											SET_INTEGER_ELT(column, j, elt);
										else
											break;
									}
									classgets(column, variable_lng_class_rvector);
									break;
								case VariableDescriptor::Declaration::Type::BIN: {
									const size_t num_bits = num_instances * var.declaration->size;
									const size_t bits_per_word = sizeof(uint32_t) * 8;
									size_t num_words = num_bits / bits_per_word;
									if (num_bits % bits_per_word != 0)
										++num_words;

									PROTECT(column = allocVector(INTSXP, num_words));
									for (int j = 0; j < num_words; ++j) {
										uint32_t elt = fread_BIN(rbf_file);
										if (rbf_file)
											SET_INTEGER_ELT(column, j, elt);
										else
											break;
									}
									setAttrib(column, binpck_instance_num, ScalarInteger(num_instances));
									setAttrib(column, binpck_instance_len, ScalarInteger(var.declaration->size));
									classgets(column, variable_bin_class_rvector);
									break;
								}
								case VariableDescriptor::Declaration::Type::PCK: {
									const size_t num_bits = num_instances * var.declaration->size;
									const size_t bits_per_word = sizeof(uint32_t) * 8;
									size_t num_words = num_bits / bits_per_word;
									if (num_bits % bits_per_word != 0)
										++num_words;

									PROTECT(column = allocVector(INTSXP, num_words));
									for (int j = 0; j < num_words; ++j) {
										uint32_t elt = fread_PCK(rbf_file);
										if (rbf_file)
											SET_INTEGER_ELT(column, j, elt);
										else
											break;
									}
									setAttrib(column, binpck_instance_num, ScalarInteger(num_instances));
									setAttrib(column, binpck_instance_len, ScalarInteger(var.declaration->size));
									classgets(column, variable_pck_class_rvector);
									break;
								}
								default:
									PROTECT(column = allocVector(VECSXP, 0));
							}
						}
						else {
							PROTECT(column = allocVector(VECSXP, 0));
						}
					}
					else {
						PROTECT(column = allocVector(VECSXP, 0));
					}
					SET_STRING_ELT(names, i+1, mkChar(var.name.c_str()));
					SET_VECTOR_ELT(current_entity, i+1, column);

					setAttrib(column, description_rsymbol, mkString(var.description.c_str()));
					if (var.descriptor.missing) {
						setAttrib(column, missing_rsymbol, ScalarInteger(*var.descriptor.missing));
					}
					if (var.descriptor.not_applicable) {
						setAttrib(column, notapplicable_rsymbol, ScalarInteger(*var.descriptor.not_applicable));
					}
					/*if (var.labels.size() > 0) {
						int num_levels = var.labels.size();
						if (var.descriptor.missing)
							++num_levels;
						if (var.descriptor.not_applicable)
							++num_levels;
						RAIISEXP levels = allocVector(STRSXP, num_levels);
						SET_STRING_ELT(levels, 0, mkChar("[NA]"));
						for (int i = 0; i < num_levels; ++i)
							SET_STRING_ELT(levels, i+1, mkChar(var.labels[i].c_str()));
						setAttrib(column, R_LevelsSymbol, levels);
					}*/
					std::stringstream ss;
					ss << var;
					setAttrib(column, documentation_rsymbol, mkString(ss.str().c_str()));
					UNPROTECT(1); // column
				}
			}
		}
	}

	namesgets(ans, ans_names);
	return ans;
}
} // extern "C"
