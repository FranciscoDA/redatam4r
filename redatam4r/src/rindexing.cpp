#include <algorithm>
#include <redatam/read_rbf.h>

#include "rindexing.h"
#include "protected_sexp.h"
#include <R.h>
#include <Rdefines.h>

// translate integer values from `dst` into
// the indices of their corresponding matches in `levels`
// this is necessary because REDATAM can
// store factors as sparse, 0-based integers
// while R requires factors to be contiguous and 1-based
void translate_factor(SEXP dst, SEXP levels) {
	const int* first = INTEGER_RO(levels);
	const int* last = first + length(levels);
	for (int i = 0; i < length(dst); ++i) {
		int v = INTEGER_ELT(dst, i);
		const int* pos = std::lower_bound(first, last, v);
		SET_INTEGER_ELT(dst, i, pos != last && *pos==v ? pos-first+1 : NA_INTEGER);
	}
}

// if `src` has `levels` and `levelkeys` attributes, then
// translate the values in `dst` to the factors in `src`
// using `translate_factor` and set the `levels` and `class`
// attributes of `dst` to turn it into a proper factor object
void configure_factor(SEXP dst, SEXP src) {
	SEXP labels = getAttrib(src, R_LevelsSymbol);
	SEXP levels = getAttrib(src, mkString("levelkeys"));
	if (!isNull(levels) and !isNull(labels)) {
		translate_factor(dst, levels);
		setAttrib(dst, R_LevelsSymbol, labels);
		classgets(dst, mkString("factor"));
	}
}

std::string get_rbf_path(SEXP robj) { return CHAR(asChar(STRING_ELT(getAttrib(robj, mkString("data.path")), 0))); }
size_t get_instance_len(SEXP robj) { return INTEGER_ELT(getAttrib(robj, mkString("instance.len")), 0); }
int get_instance_num(SEXP robj) { return INTEGER_ELT(getAttrib(robj, mkString("instance.num")), 0); }

class StrIteratorImpl {
private:
	class Proxy {
	public:
		Proxy(SEXP sexp, int i) : sexp(sexp), i(i) {}
		Proxy& operator=(const std::string& s) {
			SET_STRING_ELT(sexp, i, mkChar(s.c_str()));
			return *this;
		}
		Proxy& operator=(const SEXP& sexp) {
			SET_STRING_ELT(sexp, i, sexp);
			return *this;
		}
	private:
		SEXP sexp;
		int i;
	};

public:
	StrIteratorImpl(SEXP sexp, int i) : sexp(sexp), i(i) {}
	StrIteratorImpl(SEXP sexp) : sexp(sexp) {}
	Proxy operator*() { return Proxy{sexp, i}; }
	StrIteratorImpl& operator++() {
		++i;
		return *this;
	}
	bool operator!=(const StrIteratorImpl& other) {
		return sexp == other.sexp and i == other.i;
	}
	StrIteratorImpl operator+(int j) { return StrIteratorImpl{sexp, i+j}; }
private:
	SEXP sexp;
	int i = 0;
};
StrIteratorImpl StrIterator(SEXP sexp) { return StrIteratorImpl(sexp); }
int* IntIterator(SEXP sexp) { return INTEGER(sexp); }
double* RealIterator(SEXP sexp) { return REAL(sexp); }

template <typename F> struct ReadingFunctionSignature {
	using type1 = void(*)(const int*, const int*, typename std::result_of<F(SEXP)>::type, Redatam::fs::path, size_t);
	using type2 = void(*)(const int*, const int*, typename std::result_of<F(SEXP)>::type, Redatam::fs::path);
};

template<typename F, typename NaType>
SEXP get_rvector(SEXP robj, SEXP indices, F&& iter, typename ReadingFunctionSignature<F>::type1 func, NaType na) {
	int instance_len = get_instance_len(robj);
	ProtectedSEXP ans = allocVector(TYPEOF(robj), length(indices));
	auto output = iter(ans);
	const int* first = INTEGER_RO(indices);

	std::fill(output, output+length(ans), na);
	func(first, first+length(indices), output, get_rbf_path(robj), instance_len);

	if (TYPEOF(ans) == INTSXP)
		configure_factor(ans, robj);

	return ans;
}
template<typename F, typename NaType>
SEXP get_rvector(SEXP robj, SEXP indices, F&& iter, typename ReadingFunctionSignature<F>::type2 func, NaType na) {
	ProtectedSEXP ans = allocVector(TYPEOF(robj), length(indices));
	auto output = iter(ans);
	const int* first = INTEGER_RO(indices);

	std::fill(output, output+length(ans), na);
	func(first, first+length(indices), output, get_rbf_path(robj));

	if (TYPEOF(ans) == INTSXP)
		configure_factor(ans, robj);

	return ans;
}

extern "C" {

SEXP bin_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &IntIterator, &Redatam::read_bin_rbf, NA_INTEGER);
}

SEXP pck_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &IntIterator, &Redatam::read_pck_rbf, NA_INTEGER);
}

SEXP chr_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &StrIterator, &Redatam::read_chr_rbf, NA_STRING);
}

SEXP int_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &IntIterator, &Redatam::read_int_rbf, NA_INTEGER);
}

SEXP lng_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &IntIterator, &Redatam::read_long_rbf, NA_INTEGER);
}

SEXP real_get_rvector(SEXP robj, SEXP indices) {
	return get_rvector(robj, indices, &RealIterator, &Redatam::read_double_rbf, NA_REAL);
}

}
