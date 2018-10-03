#include <iostream>
#include <fstream>
#include <string>
#include "../primitives.h"
#include "../util.h"
#include "entity_descriptor.h"

EntityDescriptor EntityDescriptor::fread(std::istream& stream, bool is_root) {
	EntityDescriptor d;
	for (; d.name1.empty(); d.name1 = fread_string(stream)) {}

	if (!is_root)
		d.name2 = fread_string(stream);
	d.parent       = fread_string(stream);
	d.description  = fread_string(stream);
	d.ptr_path = DOSPath(fread_string(stream));
	d.unknown1     = fread_uint16_t(stream);
	d.id_varname    = fread_string(stream);
	d.name_varname  = fread_string(stream);
	d.documentation = fread_string(stream);
	d.num_entities  = fread_uint16_t(stream);
	for (auto& c : d.unknown2) c = stream.get();
	d.num_vars      = fread_uint16_t(stream);
	d.unknown3      = fread_uint16_t(stream);
	d.is_selectable = fread_uint16_t(stream);
	for (auto& c : d.unknown6) c = stream.get();
	d.weight_varname     = fread_string(stream);
	d.label              = fread_string(stream);
	d.is_sensible        = fread_uint16_t(stream);
	d.sensible_threshold = fread_uint16_t(stream);
	return d;
}

std::ostream& operator<<(std::ostream& stream, const EntityDescriptor& d) {
	return stream
		<< "Name1: " << d.name1
		<< "\nName2: " << d.name2
		<< "\nParent: " << d.parent
		<< "\nDescription: " << d.description
		<< "\nPTR file: " << d.ptr_path
		<< "\nVar ID: " << d.id_varname
		<< "\nVar Name: " << d.name_varname
		<< "\nDocumentation: " << d.documentation
		<< "\nNum Vars: " << d.num_vars
		<< "\nWeighting variable: " << d.weight_varname
		<< "\nLabel: " << d.label
		<< "\nSelectable: " << (d.is_selectable ? "Yes" : "No")
		<< "\nUNK2: " << d.unknown2
		<< "\nUNK6: " << d.unknown6;
}

