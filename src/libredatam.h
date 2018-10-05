#ifndef _REDATAM4R_LIBREDATAM_H_
#define _REDATAM4R_LIBREDATAM_H_

#include <R/R.h>
#include <R/Rinternals.h>

extern "C" {
	SEXP read_redatam(SEXP dic_path);
}

#endif
