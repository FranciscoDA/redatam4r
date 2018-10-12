#ifndef _REDATAM4R_RINDEXING_H_
#define _REDATAM4R_RINDEXING_H_

#include <R/R.h>
#include <R/Rinternals.h>

extern "C" {
	SEXP bin_get_rvector(SEXP bin, SEXP indices);
	SEXP pck_get_rvector(SEXP pck, SEXP indices);
	SEXP chr_get_rvector(SEXP chr, SEXP indices);
	SEXP int_get_rvector(SEXP intg, SEXP indices);
	SEXP lng_get_rvector(SEXP lng, SEXP indices);
	SEXP real_get_rvector(SEXP real, SEXP indices);
}

#endif // _REDATAM4R_RINDEXING_H_
