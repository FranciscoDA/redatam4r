#include <string>
#include <algorithm>
#include "redatam/util.h"

namespace Redatam {

std::string tolower(const std::string& s) {
	std::string r;
	r.reserve(s.size());
	std::transform(begin(s), end(s), back_inserter(r), [](char ch) { return std::tolower(ch); });
	return r;
}

fs::path locate_icase(const fs::path& path) {
	std::string base_lower = tolower(path.filename().string());
	for (auto& p : fs::directory_iterator(path.parent_path()))
		if (base_lower == tolower(p.path().filename()))
			return path.parent_path() / p.path().filename();
	return {};
}

} // namespace Redatam
