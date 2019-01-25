#ifndef _REDATAM4R_RINDEXING_H_
#define _REDATAM4R_RINDEXING_H_

#include <R/Rinternals.h>

extern "C" {
	SEXP bin_get_rvector(SEXP robj, SEXP indices);
	SEXP pck_get_rvector(SEXP robj, SEXP indices);
	SEXP chr_get_rvector(SEXP robj, SEXP indices);
	SEXP int_get_rvector(SEXP robj, SEXP indices);
	SEXP lng_get_rvector(SEXP robj, SEXP indices);
	SEXP real_get_rvector(SEXP robj, SEXP indices);
}

#endif // _REDATAM4R_RINDEXING_H_
