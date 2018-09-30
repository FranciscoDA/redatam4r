#ifndef _REDATAM4R_UTIL_H_
#define _REDATAM4R_UTIL_H_

#include <string>
#include <fstream>
#include <array>
#include <iomanip>

std::string tolower(const std::string& s);
std::string locate_icase(const std::string& dir, const std::string& base);
std::string dos_basename(const std::string& s);

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& arr) {
	for (int c : arr)
		stream << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0') << c << " ";
	return stream << std::setbase(10) << std::setfill('0');
}


#endif
