#pragma once

#if __cplusplus >= 201703L
#include <experimental/filesystem>
namespace Redatam {
namespace fs = std::experimental::filesystem;
}
#else
#include <boost/filesystem.hpp>
namespace Redatam {
namespace fs = boost::filesystem;
}
#endif
