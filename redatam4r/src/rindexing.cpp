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

std::string get_rbf_path(SEXP robj) { return CHAR(asChar(getAttrib(robj, mkString("data.path")))); }
size_t get_instance_len(SEXP robj) { return asInteger(getAttrib(robj, mkString("instance.len"))); }
int get_instance_num(SEXP robj) { return asInteger(getAttrib(robj, mkString("instance.num"))); }

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
	StrIteratorImpl operator++(int) {
		auto copy = *this;
		++i;
		return copy;
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

template<typename F, typename NaType>
SEXP get_rvector_variable(SEXP robj, F&& iterator_generator, void (*rbf_reader)(typename std::result_of<F(SEXP)>::type, std::istream&, size_t, size_t), NaType na) {
	int instance_len = get_instance_len(robj);
	int count        = get_instance_num(robj);
	std::string path = get_rbf_path(robj);

	ProtectedSEXP ans = allocVector(TYPEOF(robj), count);
	auto output = iterator_generator(ans);
	std::fill(output, output+length(ans), na);

	std::fstream stream(path, std::ios_base::in|std::ios_base::binary);
	rbf_reader(output, stream, count, instance_len);

	if (TYPEOF(ans) == INTSXP)
		configure_factor(ans, robj);
	return ans;
}
template<typename F, typename NaType>
SEXP get_rvector_fixed(SEXP robj, F&& iterator_generator, void (*rbf_reader)(typename std::result_of<F(SEXP)>::type, std::istream&, size_t), NaType na) {
	int count        = get_instance_num(robj);
	std::string path = get_rbf_path(robj);

	ProtectedSEXP ans = allocVector(TYPEOF(robj), count);
	auto output = iterator_generator(ans);
	std::fill(output, output+length(ans), na);

	std::fstream stream(path, std::ios_base::in|std::ios_base::binary);
	rbf_reader(output, stream, count);

	if (TYPEOF(ans) == INTSXP)
		configure_factor(ans, robj);
	return ans;
}

extern "C" {

SEXP _redatam4r_bin_get_rvector(SEXP robj) {
	return get_rvector_variable(robj, &IntIterator, &Redatam::read_bin_rbf, NA_INTEGER);
}

SEXP _redatam4r_pck_get_rvector(SEXP robj) {
	return get_rvector_variable(robj, &IntIterator, &Redatam::read_pck_rbf, NA_INTEGER);
}

SEXP _redatam4r_chr_get_rvector(SEXP robj) {
	return get_rvector_variable(robj, &StrIterator, &Redatam::read_chr_rbf<StrIteratorImpl>, NA_STRING);
}

SEXP _redatam4r_int_get_rvector(SEXP robj) {
	return get_rvector_fixed(robj, &IntIterator, &Redatam::read_int_rbf<int*>, NA_INTEGER);
}

SEXP _redatam4r_lng_get_rvector(SEXP robj) {
	return get_rvector_fixed(robj, &IntIterator, &Redatam::read_long_rbf<int*>, NA_INTEGER);
}

SEXP _redatam4r_real_get_rvector(SEXP robj) {
	return get_rvector_fixed(robj, &RealIterator, &Redatam::read_double_rbf<double*>, NA_REAL);
}

}
