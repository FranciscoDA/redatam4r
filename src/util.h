#ifndef _REDATAM4R_UTIL_H_
#define _REDATAM4R_UTIL_H_

#include <string>
#include <fstream>
#include <array>
#include <iomanip>

std::string tolower(const std::string& s);
std::string locate_icase(const std::string& dir, const std::string& base);

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& arr) {
	for (int c : arr)
		stream << "0x" << std::setbase(16) << std::setw(2) << std::setfill('0') << c << " ";
	return stream << std::setbase(10) << std::setfill('0');
}

void rtrim(std::string& s);

class Path {
public:
	Path() = delete;
	std::string basename() const;
	Path dir() const;
	const std::string& as_string() const;
	Path& operator+=(const std::string& component);
	Path operator+(const std::string& component) const;
	bool empty() const;
	friend std::ostream& operator<<(std::ostream& stream, const Path& p);
protected:
	Path(const std::string& s, char sep);
	Path(const char* s, char sep);
private:
	std::string raw_path;
	char sep;
};
std::ostream& operator<<(std::ostream& stream, const Path& p);

class DOSPath : public Path {
public:
	DOSPath(const std::string& s) : Path(s, '\\') {}
	DOSPath(const char* s) : Path(s, '\\') {}
	DOSPath() : Path("", '\\') {}
};
class UnixPath : public Path {
public:
	UnixPath(const std::string& s) : Path(s, '/') {}
	UnixPath(const char* s) : Path(s, '/') {}
	UnixPath() : Path("", '/') {}
};

#endif
