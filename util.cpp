#include <string>
#include <algorithm>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include "util.h"

std::string tolower(const std::string& s) {
	std::string r = s;
	for (auto& c : r) c = std::tolower(c);
	return r;
}

std::string locate_icase(const std::string& dir, const std::string& base) {
	std::string base_lower = tolower(base);

	DIR* d = opendir(dir.c_str());
	for (struct dirent* de; de = readdir(d);) {
		if (base_lower == tolower(de->d_name)) {
			closedir(d);
			return std::string(de->d_name);
		}
	}
	closedir(d);
	return std::string();
}

std::string dos_basename(const std::string& s) {
	return s.substr(s.rfind('\\')+1, std::string::npos);
}
