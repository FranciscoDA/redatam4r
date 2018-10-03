#include <fstream>
#include <string>
#include <regex>
#include <optional>
#include "../primitives.h"
#include "../util.h"
#include "variable_descriptor.h"

std::optional<VariableDescriptor::Declaration> VariableDescriptor::Declaration::fromDeclarationString(const std::string& declstr) {
	std::regex r ("DATASET (BIN|CHR|DBL|INT|LNG|PCK) '([^']+)' SIZE ([0-9]+)", std::regex::extended);
	if (std::smatch match; std::regex_match(declstr, match, r)) {
		Declaration::Type t = Declaration::Type::CHR;
		if (match[1] == "BIN") t = Declaration::Type::BIN;
		else if (match[1] == "CHR") t = Declaration::Type::CHR;
		else if (match[1] == "DBL") t = Declaration::Type::DBL;
		else if (match[1] == "INT") t = Declaration::Type::INT;
		else if (match[1] == "LNG") t = Declaration::Type::LNG;
		else if (match[1] == "PCK") t = Declaration::Type::PCK;
		std::string p = match[2];
		size_t s = std::stoi(match[3]);
		return Declaration{t, p, s};
	}
	else {
		return std::nullopt;
	}
}
VariableDescriptor VariableDescriptor::fread(std::istream& stream) {
	VariableDescriptor d;
	d.name = fread_string(stream);
	d.declaration = VariableDescriptor::Declaration::fromDeclarationString(fread_string(stream));
	d.filter   = fread_string(stream);
	d.range    = fread_string(stream);
	d.datatype = fread_string(stream);
	d.labels   = fread_string(stream);
	d.description   = fread_string(stream);
	d.descriptors   = fread_string(stream);
	d.unknown1 = fread_uint16_t(stream);
	d.documentation = fread_string(stream);
	d.id = fread_uint16_t(stream);
	for (auto& c : d.unknown) c = stream.get();
	return d;
}

std::ostream& operator<<(std::ostream& stream, const VariableDescriptor& d) {
	return stream
		<< "Name: " << d.name
		<< "\nDeclaration: " << (d.declaration ? d.declaration->rbf_path : "(null)")
		<< "\nFilter: " << d.filter
		<< "\nRange: " << d.range
		<< "\nDatatype: " << d.datatype
		<< "\nLabels: " << d.labels
		<< "\nDescription: " << d.description
		<< "\nDescriptors: " << d.descriptors
		<< "\nUNK1: " << d.unknown1
		<< "\nDocumentation: " << d.documentation
		<< "\nID: " << d.id
		<< "\nUNK: " << d.unknown;
}

