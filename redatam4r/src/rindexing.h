#pragma once

#include <R/Rinternals.h>

extern "C" {
	SEXP _redatam4r_bin_get_rvector(SEXP robj);
	SEXP _redatam4r_pck_get_rvector(SEXP robj);
	SEXP _redatam4r_chr_get_rvector(SEXP robj);
	SEXP _redatam4r_int_get_rvector(SEXP robj);
	SEXP _redatam4r_lng_get_rvector(SEXP robj);
	SEXP _redatam4r_real_get_rvector(SEXP robj);
}
