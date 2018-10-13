#include <string>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include "util.h"

std::string tolower(const std::string& s) {
	std::string r = s;
	for (auto& c : r) c = std::tolower(c);
	return r;
}

std::string locate_icase(const std::string& dir, const std::string& base) {
	std::string result;
	std::string base_lower = tolower(base);
	DIR* d = opendir(dir.c_str());
	for (struct dirent* de; (de = readdir(d));) {
		if (base_lower == tolower(de->d_name)) {
			result.assign(de->d_name);
			break;
		}
	}
	closedir(d);
	return result;
}

void rtrim(std::string& s) {
	auto it = s.end();
	--it;
	while(it != s.begin() and std::isspace(*it)) {
		--it;
	}
	++it;
	s.assign(s.begin(), it);
}

std::ostream& operator<<(std::ostream& stream, const Path& p) {
	return stream << p.raw_path;
}
Path::Path(const std::string& s, char sep) : raw_path(s), sep(sep) {}
Path::Path(const char* s, char sep) : raw_path(s), sep(sep) {}
std::string Path::basename() const {
	size_t i = raw_path.rfind(sep);
	return raw_path.substr(i+1, std::string::npos);
}
Path Path::dir() const {
	size_t i = raw_path.rfind(sep);
	return Path(raw_path.substr(0, i), sep);
}
const std::string& Path::as_string() const { return raw_path; }
Path& Path::operator+=(const std::string& component) {
	(raw_path += sep) += component;
	return *this;
}
Path Path::operator+(const std::string& component) const {
	Path copy = *this;
	return copy += component;
}
bool Path::empty() const { return raw_path.empty(); }

