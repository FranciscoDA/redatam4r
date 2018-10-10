#ifndef _REDATAM4R_PRIMITIVES_H_
#define _REDATAM4R_PRIMITIVES_H_

#include <fstream>
#include <string>
#include <vector>

/* read and return value */
uint8_t fread_uint8_t(std::istream& stream);
uint16_t fread_uint16_t(std::istream& stream);
uint32_t fread_uint32_t(std::istream& stream);
std::string fread_string(std::istream& stream);
std::string fread_fixed_string(std::istream& stream, size_t width);
uint32_t fread_PCK(std::istream& stream);
uint32_t fread_BIN(std::istream& stream);

size_t fread_decimal(std::istream& stream, std::size_t digits);
double fread_double(std::istream& stream);

#endif
