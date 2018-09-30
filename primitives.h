#ifndef _REDATAM4R_PRIMITIVES_H_
#define _REDATAM4R_PRIMITIVES_H_

#include <fstream>
#include <string>

uint8_t fread_uint8_t(std::istream& stream);
uint16_t fread_uint16_t(std::istream& stream);
uint32_t fread_uint32_t(std::istream& stream);
std::string fread_string(std::istream& stream);
size_t fread_decimal(std::istream& stream, std::size_t digits);
double fread_double(std::istream& stream);

#endif
