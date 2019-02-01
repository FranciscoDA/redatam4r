#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "fs.h"

namespace Redatam {

uint8_t fread_uint8_t(std::istream& stream);
uint16_t fread_uint16_t(std::istream& stream);
uint32_t fread_uint32_t(std::istream& stream);

// reads a pascal-style string (leading uint_16 LE for length) encoded in windows 1252
std::string fread_string(std::istream& stream);

fs::path fread_path(std::istream& stream);

// reads a fixed width string encoded in windows 1252
std::string fread_fixed_string(std::istream& stream, size_t width);

// reads a fixed number of ascii digits
size_t fread_decimal(std::istream& stream, size_t digits);

double fread_double(std::istream& stream);

// reads a word of PCK encoded data (32 bits LE)
uint32_t fread_PCK(std::istream& stream);

// reads a word of BIN encoded data (32 bits BE)
uint32_t fread_BIN(std::istream& stream);

} // namespace Redatam
