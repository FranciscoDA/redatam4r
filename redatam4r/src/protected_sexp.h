#ifndef _REDATAM4R_PROTECTED_SEXP_H_
#define _REDATAM4R_PROTECTED_SEXP_H_

#include <R.h>
#include <Rdefines.h>

class ProtectedSEXP {
public:
	ProtectedSEXP(SEXP v);
	ProtectedSEXP();
	~ProtectedSEXP();
	operator SEXP();
	ProtectedSEXP& operator=(SEXP v);
	ProtectedSEXP operator=(const ProtectedSEXP&) = delete;
protected:
	SEXP sexp = nullptr;
};

#endif
