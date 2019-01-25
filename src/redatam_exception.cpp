#include "redatam_exception.h"

namespace Redatam {
	Exception::Exception(const std::string& msg) : msg(msg) {}
	Exception::Exception(std::string&& msg) : msg(msg) {}

	const char* Exception::what() const noexcept {
		return msg.c_str();
	}
} // namespace Redatam
