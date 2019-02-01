#pragma once

#include "primitives.h"
#include "fs.h"

namespace Redatam {

template <typename OutputIterator>
void read_ptr(const fs::path& ptr_path, OutputIterator output) {
	std::fstream ptr_file(ptr_path, std::ios_base::in|std::ios_base::binary);
	if (!ptr_file)
		throw Exception(std::string("Could not read ptr file: ") + std::string(ptr_path));

	int parent_id = 0;
	unsigned current_accum = 0;
	unsigned previous_accum = fread_uint32_t(ptr_file); // always reads 0 at offset 0

	while (current_accum = fread_uint32_t(ptr_file), ptr_file) {
		++parent_id;
		for (unsigned i = 0; i < current_accum - previous_accum; ++i)
			*(output++) = parent_id;
		previous_accum = current_accum;
	}
}

} // namespace Redatam
