#include <functional>
#include "primitives.h"
#include "fs.h"

namespace Redatam {
	namespace detail {

	template <typename First, typename Last, typename Output, typename F>
	void read_fixed_bitwise(First first, Last last, Output output, std::istream& stream, const size_t stream_size, const size_t size, F&& fread_word) {
		const unsigned bits_per_word = sizeof(uint32_t) * 8;
		const uint32_t mask = (1 << size) - 1;

		uint32_t element = 0;
		size_t current_offset = std::numeric_limits<size_t>::max();
		for (; first != last; ++first, ++output) {
			if (*first < 0)
				continue;
			size_t target_offset = *first * size / bits_per_word * sizeof(uint32_t);
			if (target_offset >= stream_size)
				continue;

			// seek only if not contiguous. seeking causes a buffer flush
			// can be very slow for random access
			if (target_offset != current_offset) {
				stream.seekg(target_offset);
				element = fread_word(stream);
				current_offset = target_offset;
			}
			const int left_offset = *first * size % bits_per_word;

			// overshoot <= 0 indicates that the value is contained in `element`
			// overshoot > 0 indicates that the highter bits are in `element` and
			// the lower ones are in the next word
			const int overshoot = left_offset + size - bits_per_word;
 
			uint32_t v = overshoot > 0 ? element << overshoot : element >> -overshoot;
			if (overshoot > 0) {
				element = fread_word(stream);
				++current_offset;
				v += element >> (bits_per_word-overshoot);
			}
			*output = v & mask;
		}
	}
	template <typename First, typename Last, typename Output, typename F>
	void read_fixed_byte(First first, Last last, Output output, std::istream& stream, const size_t stream_size, const size_t size, F&& read_value) {
		size_t current_offset = -1;
		for (; first != last; ++first, ++output) {
			if (*first < 0) 
				continue;
			size_t target_offset = *first * size;
			if (target_offset > stream_size)
				continue;
			if (target_offset != current_offset) {
				stream.seekg(target_offset);
				current_offset = target_offset;
			}
			*output = read_value(stream);
			current_offset += size;
		}
	}

	} // namespace detail

	template <typename First, typename Last, typename Output>
	void read_bin_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size, size_t size) {
		detail::read_fixed_bitwise(first, last, output, stream, stream_size, size, &fread_BIN);
	}
	template <typename First, typename Last, typename Output>
	void read_bin_rbf(First first, Last last, Output output, fs::path path, size_t size) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_bin_rbf(first, last, output, f, fs::file_size(path), size);
	}

	template <typename First, typename Last, typename Output>
	void read_pck_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size, size_t size) {
		detail::read_fixed_bitwise(first, last, output, stream, stream_size, size, &fread_PCK);
	}
	template <typename First, typename Last, typename Output>
	void read_pck_rbf(First first, Last last, Output output, fs::path path, size_t size) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_pck_rbf(first, last, output, f, fs::file_size(path), size);
	}

	template <typename First, typename Last, typename Output>
	void read_chr_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size, size_t size) {
		detail::read_fixed_byte(first, last, output, stream, stream_size, size, std::bind(&fread_fixed_string, std::placeholders::_1, size));
	}
	template <typename First, typename Last, typename Output>
	void read_chr_rbf(First first, Last last, Output output, fs::path path, size_t size) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_chr_rbf(first, last, output, f, fs::file_size(path), size);
	}

	template <typename First, typename Last, typename Output>
	void read_int_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size) {
		detail::read_fixed_byte(first, last, output, stream, stream_size, sizeof(uint16_t), &fread_uint16_t);
	}
	template <typename First, typename Last, typename Output>
	void read_int_rbf(First first, Last last, Output output, fs::path path) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_int_rbf(first, last, output, f, fs::file_size(path));
	}

	template <typename First, typename Last, typename Output>
	void read_long_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size) {
		detail::read_fixed_byte(first, last, output, stream, stream_size, sizeof(uint32_t), &fread_uint32_t);
	}
	template <typename First, typename Last, typename Output>
	void read_long_rbf(First first, Last last, Output output, fs::path path) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_long_rbf(first, last, output, f, fs::file_size(path));
	}

	template <typename First, typename Last, typename Output>
	void read_double_rbf(First first, Last last, Output output, std::istream& stream, size_t stream_size) {
		detail::read_fixed_byte(first, last, output, stream, stream_size, sizeof(double), &fread_double);
	}
	template <typename First, typename Last, typename Output>
	void read_double_rbf(First first, Last last, Output output, fs::path path) {
		std::fstream f(path, std::ios_base::in|std::ios_base::binary);
		read_double_rbf(first, last, output, f, fs::file_size(path));
	}
}
