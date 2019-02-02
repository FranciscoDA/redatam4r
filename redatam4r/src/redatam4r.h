#ifndef _REDATAM4R_REDATAM4R_H_
#define _REDATAM4R_REDATAM4R_H_

#include <R/R.h>
#include <R/Rinternals.h>

extern "C" {
	SEXP _redatam4r_read_redatam(SEXP dic_path);
}

#endif
