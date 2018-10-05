#ifndef _REDATAM4R_PRIMITIVES_H_
#define _REDATAM4R_PRIMITIVES_H_

#include <fstream>
#include <string>

/* read and return value */
uint8_t fread_uint8_t(std::istream& stream);
uint16_t fread_uint16_t(std::istream& stream);
uint32_t fread_uint32_t(std::istream& stream);
std::string fread_string(std::istream& stream);

/* read into dst and return true if read was successful*/
std::istream& fread_uint8_t(uint8_t& dst, std::istream& stream);
std::istream& fread_uint16_t(uint16_t& dst, std::istream& stream);
std::istream& fread_uint32_t(uint32_t& dst, std::istream& stream);
std::istream& fread_string(std::string& dst, std::istream& stream);


size_t fread_decimal(std::istream& stream, std::size_t digits);
double fread_double(std::istream& stream);

#endif
