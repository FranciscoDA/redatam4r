#include "redatam4r.h"
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

extern "C" {
static const R_CMethodDef CEntries[] = {
	{"read_redatam", (DL_FUNC) &read_redatam, 1},
	{"binpck_get", (DL_FUNC) &binpck_get, 2},
	{NULL, NULL, 0}
};

void R_init_redatam4r(DllInfo* dll) {
	R_RegisterCCallable("redatam4r", "read_redatam", (DL_FUNC)&read_redatam);
	R_RegisterCCallable("redatam4r", "binpck_get", (DL_FUNC)&binpck_get);
    /*R_useDynamicSymbols(dll, FALSE);
    R_forceSymbols(dll, TRUE);*/
}

} 
