#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <redatam/fs.h>
#include <redatam/read_dic.h>
#include <redatam/read_ptr.h>
#include <redatam/dic/dictionary_descriptor.h>
#include <redatam/dic/entity_descriptor.h>
#include <redatam/dic/variable_descriptor.h>
#include "protected_sexp.h"
#include <R.h>
#include <Rdefines.h>

template<typename T>
SEXP stream_to_R(const T& obj) {
	std::stringstream ss;
	ss << obj << '\n';
	return mkString(ss.str().c_str());
}

extern "C" {
SEXP read_redatam(SEXP dic_path_in) {
	Redatam::fs::path dic_path {CHAR(asChar(dic_path_in))};

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

	ProtectedSEXP ans, ans_names;
	ProtectedSEXP current_entity, current_entity_names;

	Redatam::read_dic(dic_path,
		// dic + root entity callback
		[&](Redatam::DictionaryDescriptor& dic, Redatam::EntityDescriptor& root_entity) {
			ans = allocVector(VECSXP, root_entity.num_entities-1);
			ans_names = allocVector(STRSXP, root_entity.num_entities-1);
			setAttrib(ans, description_rsymbol, stream_to_R(dic));
			classgets(ans, mkString("redatam.database"));
			namesgets(ans, ans_names);
		},
		// entity callback
		[&](Redatam::EntityDescriptor& entity, size_t i) {
			SET_STRING_ELT(ans_names, i-1, mkChar(entity.name1.c_str()));

			current_entity = allocVector(VECSXP, entity.num_vars+1);
			current_entity_names = allocVector(STRSXP, entity.num_vars+1);

			ProtectedSEXP column0 = allocVector(INTSXP, entity.num_instances);
			Redatam::read_ptr(entity.real_ptr_path, INTEGER(column0));

			std::string column0_description = "Parent instance row in " + entity.parent + "(surrogate attribute)\n";
			setAttrib(column0, description_rsymbol, mkString(column0_description.c_str()));
			SET_VECTOR_ELT(current_entity, 0, column0);
			SET_STRING_ELT(current_entity_names, 0, mkChar((entity.parent + "_id").c_str()));

			// compact row names representation (see .set_row_names and .row_names_info)
			ProtectedSEXP current_entity_rownames = allocVector(INTSXP, 2);
			SET_INTEGER_ELT(current_entity_rownames, 0, R_NaInt);
			SET_INTEGER_ELT(current_entity_rownames, 1, -entity.num_instances);
			setAttrib(current_entity, R_RowNamesSymbol, current_entity_rownames);
			
			classgets(current_entity, entity_class_rvector);
			namesgets(current_entity, current_entity_names);
			setAttrib(current_entity, description_rsymbol, stream_to_R(entity));
			SET_VECTOR_ELT(ans, i-1, current_entity);
		},
		// variable callback
		[&](Redatam::VariableDescriptor& variable, Redatam::EntityDescriptor& entity, size_t i) {
			// create a column vector for this variable of the appropiate type and class
			int column_sexptype = -1;
			SEXP column_class = nullptr;
			switch (variable.declaration->type) {
				case Redatam::VariableDescriptor::Declaration::Type::DBL:
					column_sexptype = REALSXP;
					column_class = variable_dbl_class_rvector;
					break;
				case Redatam::VariableDescriptor::Declaration::Type::CHR:
					column_sexptype = STRSXP;
					column_class = variable_chr_class_rvector;
					break;
				case Redatam::VariableDescriptor::Declaration::Type::INT:
					column_sexptype = INTSXP;
					column_class = variable_int_class_rvector;
					break;
				case Redatam::VariableDescriptor::Declaration::Type::LNG:
					column_sexptype = INTSXP;
					column_class = variable_lng_class_rvector;
					break;
				case Redatam::VariableDescriptor::Declaration::Type::BIN:
					column_sexptype = INTSXP;
					column_class = variable_bin_class_rvector;
					break;
				case Redatam::VariableDescriptor::Declaration::Type::PCK:
					column_sexptype = INTSXP;
					column_class = variable_pck_class_rvector;
					break;
			}
			ProtectedSEXP column = allocVector(column_sexptype, 0);
			classgets(column, column_class);

			// fill data needed for indexing: datatype size, number of instances and rbf file path
			setAttrib(column, instance_len_rsymbol, ScalarInteger(variable.declaration->size));
			setAttrib(column, instance_num_rsymbol, ScalarInteger(entity.num_instances));
			setAttrib(column, data_path_rsymbol, mkString(std::string(variable.real_rbf_path).c_str()));

			SET_STRING_ELT(current_entity_names, i+1, mkChar(variable.name.c_str()));
			SET_VECTOR_ELT(current_entity, i+1, column);
			setAttrib(column, description_rsymbol, stream_to_R(variable));

			// fill data needed to convert to factor: levels and labels
			if (variable.labels.size() > 0 and column_sexptype == INTSXP) {
				auto levels = variable.labels;

				if (variable.descriptor.missing)
					levels.emplace_back(*variable.descriptor.missing, "<MISS>");

				if (variable.descriptor.not_applicable)
					levels.emplace_back(*variable.descriptor.not_applicable, "<NOAP>");

				std::sort(levels.begin(), levels.end());

				ProtectedSEXP labels_rvector = allocVector(STRSXP, levels.size());
				ProtectedSEXP levels_rvector = allocVector(INTSXP, levels.size());
				for (size_t i = 0; i < levels.size(); ++i) {
					SET_STRING_ELT(labels_rvector, i, mkChar(levels[i].second.c_str()));
					SET_INTEGER_ELT(levels_rvector, i, levels[i].first);
				}
				setAttrib(column, R_LevelsSymbol, labels_rvector);
				setAttrib(column, levelkeys_rsymbol, levels_rvector);
			}
		}
	);
	return ans;
}
} // extern "C"
