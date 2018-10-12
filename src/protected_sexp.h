#ifndef _REDATAM4R_PROTECTED_SEXP_H_
#define _REDATAM4R_PROTECTED_SEXP_H_

#include <R.h>
#include <Rdefines.h>

class ProtectedSEXP {
public:
	ProtectedSEXP() = delete;
	ProtectedSEXP(SEXP v);
	~ProtectedSEXP();
	operator SEXP();
protected:
	SEXP sexp;
};

#endif
