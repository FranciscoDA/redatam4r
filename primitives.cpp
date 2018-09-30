#include <iostream>
#include <fstream>
#include <string>

#include "primitives.h"

uint8_t fread_uint8_t(std::istream& stream) {
	return stream.get();
}

uint16_t fread_uint16_t(std::istream& stream) {
	return stream.get() + (stream.get() << 8);
}

uint32_t fread_uint32_t(std::istream& stream) {
	return stream.get() + (stream.get() << 8) + (stream.get() << 16) + (stream.get() << 24);
}

std::string fread_string(std::istream& stream) {
	size_t sz = fread_uint16_t(stream);
	std::string result;
	if (sz == 0)
		return result;
	if (sz == 0xFFFF)
		sz = fread_uint32_t(stream);
	char buf[sz];
	stream.read(buf, sz);
	result.reserve(sz);
	// convert windows1252 to utf8
	for (unsigned char c : buf) {
		if (c > 0x7f) {
			result += static_cast<char>(0b11000000 | ((c & 0b11000000) >> 6));
			result += static_cast<char>(0b10000000 | (c & 0b00111111));
		}
		else
			result += static_cast<char>(c);
	}
	return result;
}

size_t fread_decimal(std::istream& stream, size_t digits) {
	size_t result = 0;
	for (int i = 0; i < digits; ++i)
		result = result * 10 + (fread_uint8_t(stream)-'0');
	return result;
}

double fread_double(std::istream& stream) {
	double d;
	stream.read((char*)&d, sizeof(double));
	return d;
}

