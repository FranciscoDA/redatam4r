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

DOSPath::DOSPath(const std::string& s) : raw_path(s) {}
DOSPath::DOSPath(const char* s) : raw_path(s) {}
DOSPath::DOSPath() : raw_path() {}

std::string DOSPath::basename() const {
	size_t i = this->raw_path.rfind('\\');
	return this->raw_path.substr(i+1, std::string::npos);
}
DOSPath DOSPath::dir() const {
	size_t i = this->raw_path.rfind('\\');
	return {this->raw_path.substr(0, i)};
}
DOSPath::operator std::string() const {
	return this->raw_path;
}
DOSPath& DOSPath::operator+=(const std::string& component) {
	if (empty()) {
		this->raw_path += ".";
	}
	this->raw_path += '\\';
	this->raw_path += component;
	return *this;
}
DOSPath DOSPath::operator+(const std::string& component) const {
	DOSPath copy = *this;
	return copy += component;
}
bool DOSPath::empty() const {
	return this->raw_path.empty();
}
std::ostream& operator<<(std::ostream& stream, const DOSPath& p) {
	return stream << p.raw_path;
}

UnixPath::UnixPath(const std::string& s) : raw_path(s) {}
UnixPath::UnixPath(const char* s) : raw_path(s) {}
UnixPath::UnixPath() {}
std::string UnixPath::basename() const {
	size_t i = this->raw_path.rfind('/');
	return this->raw_path.substr(i+1, std::string::npos);
}
UnixPath UnixPath::dir() const {
	size_t i = this->raw_path.rfind('/');
	return {this->raw_path.substr(0, i)};
}
UnixPath::operator std::string() const {
	return this->raw_path;
}
UnixPath& UnixPath::operator+=(const std::string& component) {
	if (empty()) {
		this->raw_path += ".";
	}
	this->raw_path += '/';
	this->raw_path += component;
	return *this;
}
UnixPath UnixPath::operator+(const std::string& component) const {
	UnixPath copy = *this;
	return copy += component;
}
bool UnixPath::empty() const {
	return this->raw_path.empty();
}
std::ostream& operator<<(std::ostream& stream, const UnixPath& p) {
	return stream << p.raw_path;
}
