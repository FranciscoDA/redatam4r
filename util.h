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

class DOSPath {
public:
	DOSPath(const std::string& s);
	DOSPath(const char* s);
	DOSPath();
	std::string basename() const;
	DOSPath dir() const;
	operator std::string() const;
	DOSPath& operator+=(const std::string& component);
	DOSPath operator+(const std::string& component) const;
	bool empty() const;
	friend std::ostream& operator<<(std::ostream&, const DOSPath&);
private:
	std::string raw_path;
};
std::ostream& operator<<(std::ostream& stream, const DOSPath& p);

class UnixPath {
public:
	UnixPath(const std::string& s);
	UnixPath(const char* s);
	UnixPath();
	std::string basename() const;
	UnixPath dir() const;
	operator std::string() const;
	UnixPath& operator+=(const std::string& component);
	UnixPath operator+(const std::string& component) const;
	bool empty() const;
	friend std::ostream& operator<<(std::ostream&, const UnixPath&);
private:
	std::string raw_path;
};
std::ostream& operator<<(std::ostream& stream, const UnixPath& p);


#endif
