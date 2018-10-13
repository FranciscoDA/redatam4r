
#include "primitives.h"
#include "util.h"
#include "rindexing.h"
#include "protected_sexp.h"
#include <R.h>
#include <Rdefines.h>

// translate the integer values from dst into
// the indices of their corresponding matches in levelkeys
// this is done because redatam will usually
// store factors as sparse 0-based integers
// while R requires factors to be contiguous and 1-based
void translate_factor(SEXP dst, SEXP levelkeys) {
	for (int i = 0; i < length(dst); ++i) {
		int v = INTEGER_ELT(dst, i);

		int left = 0;
		int right = length(levelkeys)-1;
		while (left < right) {
			int mid = (left+right) >> 1;
			if (v < INTEGER_ELT(levelkeys, mid)) {
				right = mid - 1;
			}
			else if (INTEGER_ELT(levelkeys, mid) < v) {
				left = mid + 1;
			}
			else {
				left = mid;
				break;
			}
		}
		if (INTEGER_ELT(levelkeys, left) == v)
			SET_INTEGER_ELT(dst, i, left+1);
		else
			SET_INTEGER_ELT(dst, i, NA_INTEGER);
	}
}

// translates the factors and adds the appropiate
// object attributes
void configure_factor(SEXP dst, SEXP src) {
	SEXP levels = getAttrib(src, R_LevelsSymbol);
	if (!isNull(levels)) {
		SEXP levelkeys = getAttrib(src, mkString("levelkeys"));
		translate_factor(dst, levelkeys);
		setAttrib(dst, R_LevelsSymbol, levels);
		classgets(dst, mkString("factor"));
	}
}

extern "C" {

SEXP bin_get_rvector(SEXP bin, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(bin, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(bin, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(bin, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(INTSXP, length(indices));

	const uint32_t bits_per_word = sizeof(uint32_t) * 8;
	const uint32_t mask = (1 << instance_len) - 1;
	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index)-1;

		if (src_index < 0 or src_index >= instance_num) {
			SET_INTEGER_ELT(ans, dst_index, NA_INTEGER);
			continue;
		}

		f.seekg(src_index * instance_len / bits_per_word * sizeof(uint32_t));
		const int shifted_bits = src_index * instance_len % bits_per_word;

		if (bits_per_word - shifted_bits >= instance_len) {
			uint32_t element = fread_BIN(f);
			element >>= sizeof(uint32_t) * 8 - instance_len - shifted_bits;
			element &= mask;
			SET_INTEGER_ELT(ans, dst_index, element);
		}
		else {
			uint32_t high = fread_BIN(f);
			uint32_t low = fread_BIN(f);
			const int bits_in_low = instance_len + shifted_bits - bits_per_word;
			high <<= bits_in_low;
			low >>= bits_per_word - bits_in_low;
			SET_INTEGER_ELT(ans, dst_index, (high+low) & mask);
		}
	}
	configure_factor(ans, bin);
	return ans;
}

SEXP pck_get_rvector(SEXP pck, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(pck, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(pck, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(pck, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(INTSXP, length(indices));

	const uint32_t bits_per_word = sizeof(uint32_t) * 8;
	const uint32_t mask = (1 << instance_len) - 1;
	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index)-1;

		if (src_index < 0 or src_index >= instance_num) {
			SET_INTEGER_ELT(ans, dst_index, NA_INTEGER);
			continue;
		}

		f.seekg(src_index * instance_len / bits_per_word * sizeof(uint32_t));
		const int shifted_bits = src_index * instance_len % bits_per_word;

		if (bits_per_word - shifted_bits >= instance_len) {
			uint32_t element = fread_PCK(f);
			element >>= sizeof(uint32_t) * 8 - instance_len - shifted_bits;
			element &= mask;
			SET_INTEGER_ELT(ans, dst_index, element);
		}
		else {
			uint32_t high = fread_PCK(f);
			uint32_t low = fread_PCK(f);
			const int bits_in_low = instance_len + shifted_bits - bits_per_word;
			high <<= bits_in_low;
			low >>= bits_per_word - bits_in_low;
			SET_INTEGER_ELT(ans, dst_index, (high+low) & mask);
		}
	}
	configure_factor(ans, pck);
	return ans;
}

SEXP chr_get_rvector(SEXP chr, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(chr, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(chr, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(chr, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(STRSXP, length(indices));

	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index)-1;

		if (src_index < 0 || src_index >= instance_num) {
			SET_STRING_ELT(ans, dst_index, NA_STRING);
			continue;
		}

		f.seekg(src_index * instance_len);
		std::string s = fread_fixed_string(f, instance_len);
		rtrim(s);
		SET_STRING_ELT(ans, dst_index, mkChar(s.c_str()));
	}
	return ans;
}

SEXP int_get_rvector(SEXP intg, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(intg, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(intg, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(intg, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(INTSXP, length(indices));

	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index)-1;

		if (src_index < 0 or src_index >= instance_num) {
			SET_INTEGER_ELT(ans, dst_index, NA_INTEGER);
			continue;
		}

		f.seekg(src_index * instance_len);
		uint16_t v = fread_uint16_t(f);
		SET_INTEGER_ELT(ans, dst_index, v);
	}
	configure_factor(ans, intg);
	return ans;
}

SEXP lng_get_rvector(SEXP lng, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(lng, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(lng, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(lng, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(INTSXP, length(indices));

	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index) - 1;

		if (src_index < 0 or src_index >= instance_num) {
			SET_INTEGER_ELT(ans, dst_index, NA_INTEGER);
			continue;
		}

		f.seekg(src_index * instance_len);
		uint32_t v = fread_uint32_t(f);
		SET_INTEGER_ELT(ans, dst_index, v);
	}
	configure_factor(ans, lng);
	return ans;
}

SEXP real_get_rvector(SEXP real, SEXP indices) {
	std::string path = CHAR(asChar(STRING_ELT(getAttrib(real, mkString("data.path")), 0)));
	const int instance_len = INTEGER_ELT(getAttrib(real, mkString("instance.len")), 0);
	const int instance_num = INTEGER_ELT(getAttrib(real, mkString("instance.num")), 0);

	std::fstream f (path, std::ios_base::in | std::ios_base::binary);
	if (!f)
		error_return("could not open data file for reading");

	ProtectedSEXP ans = allocVector(INTSXP, length(indices));

	for (int dst_index = 0; dst_index < length(indices); ++dst_index) {
		int src_index = INTEGER_ELT(indices, dst_index)-1;

		if (src_index < 0 or src_index >= instance_num) {
			SET_REAL_ELT(ans, dst_index, NA_REAL);
			continue;
		}

		f.seekg(src_index * instance_len);
		double v = fread_double(f);
		SET_REAL_ELT(ans, dst_index, v);
	}
	return ans;
}

}
