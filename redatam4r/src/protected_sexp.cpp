#include "protected_sexp.h"

ProtectedSEXP::ProtectedSEXP(SEXP v) {
	sexp = PROTECT(v);
}
ProtectedSEXP::ProtectedSEXP() : sexp(nullptr) {
}
ProtectedSEXP::~ProtectedSEXP() {
	if (sexp)
		UNPROTECT_PTR(sexp);
}
ProtectedSEXP::operator SEXP() {
	return sexp;
}
ProtectedSEXP& ProtectedSEXP::operator=(SEXP v) {
	if (sexp != nullptr)
		UNPROTECT_PTR(sexp);
	sexp = PROTECT(v);
	return *this;
}
