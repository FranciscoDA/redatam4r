#include "protected_sexp.h"

ProtectedSEXP::ProtectedSEXP(SEXP v) : sexp(v) {
	PROTECT(sexp);
}
ProtectedSEXP::~ProtectedSEXP() {
	UNPROTECT(1);
}
ProtectedSEXP::operator SEXP() {
	return sexp;
}
