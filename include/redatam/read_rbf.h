#include <functional>
#include "primitives.h"
#include "fs.h"

namespace Redatam {
	namespace detail {

	template <typename Output, typename F>
	void read_fixed_bitwise(Output output, std::istream& stream, size_t count, const size_t size, F&& fread_word) {
		const unsigned bits_per_word = sizeof(uint32_t) * 8;
		const uint32_t mask = (1 << size) - 1;

		uint32_t element = fread_word(stream);
		int left_offset = 0;
		while (stream && count != 0) {
			const int overshoot = left_offset + size - bits_per_word;
			uint32_t v = overshoot > 0 ? element << overshoot : element >> -overshoot;
			if (overshoot > 0) {
				element = fread_word(stream);
				v += element >> (bits_per_word-overshoot);
			}
			*(output++) = v & mask;
			left_offset = (left_offset + size) % bits_per_word;
			--count;
		}
	}
	template <typename Output, typename F>
	void read_fixed_byte(Output output, std::istream& stream, size_t count, const size_t size, F&& read_value) {
		while (count != 0) {
			auto v = read_value(stream);
			if (!stream)
				break;
			*(output++) = std::move(v);
			--count;
		}
	}

	} // namespace detail

	template <typename Output>
	void read_bin_rbf(Output output, std::istream& stream, size_t count, size_t size) {
		detail::read_fixed_bitwise(output, stream, count, size, &fread_BIN);
	}
	template <typename Output>
	void read_pck_rbf(Output output, std::istream& stream, size_t count, size_t size) {
		detail::read_fixed_bitwise(output, stream, count, size, &fread_PCK);
	}
	template <typename Output>
	void read_chr_rbf(Output output, std::istream& stream, size_t count, size_t size) {
		detail::read_fixed_byte(output, stream, count, size, std::bind(&fread_fixed_string, std::placeholders::_1, size));
	}
	template <typename Output>
	void read_int_rbf(Output output, std::istream& stream, size_t count) {
		detail::read_fixed_byte(output, stream, count, sizeof(uint16_t), &fread_uint16_t);
	}
	template <typename Output>
	void read_long_rbf(Output output, std::istream& stream, size_t count) {
		detail::read_fixed_byte(output, stream, count, sizeof(uint32_t), &fread_uint32_t);
	}
	template <typename Output>
	void read_double_rbf(Output output, std::istream& stream, size_t count) {
		detail::read_fixed_byte(output, stream, count, sizeof(double), &fread_double);
	}
}
