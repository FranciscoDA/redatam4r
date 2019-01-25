#pragma once

#include <string>
#include <fstream>
#include <array>
#include <iomanip>
#include "fs.h"

namespace Redatam {

std::string tolower(const std::string& s);
fs::path locate_icase(const fs::path& path);

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& arr) {
	for (int c : arr)
		stream << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0') << c << " ";
	return stream << std::setbase(10) << std::setfill('0');
}

} // namespace Redatam

