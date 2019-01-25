#pragma once

#include <string>

namespace Redatam {
	class Exception : public std::exception {
		public:
			Exception(const std::string& msg);
			Exception(std::string&& msg);

			const char* what() const noexcept override;
		private:
			std::string msg;
	};
}
