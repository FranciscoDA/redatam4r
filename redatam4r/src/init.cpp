#include "redatam4r.h"
#include "rindexing.h"
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

extern "C" {
static const R_CMethodDef CEntries[] = {
	{"read_redatam", (DL_FUNC) &read_redatam, 1},
	{"bin_get_rvector", (DL_FUNC) &bin_get_rvector, 2},
	{"pck_get_rvector", (DL_FUNC) &pck_get_rvector, 2},
	{"chr_get_rvector", (DL_FUNC) &chr_get_rvector, 2},
	{"int_get_rvector", (DL_FUNC) &int_get_rvector, 2},
	{"lng_get_rvector", (DL_FUNC) &lng_get_rvector, 2},
	{"real_get_rvector", (DL_FUNC) &real_get_rvector, 2},
	{NULL, NULL, 0}
};

void R_init_redatam4r(DllInfo* dll) {
	R_RegisterCCallable("redatam4r", "read_redatam", (DL_FUNC)&read_redatam);
	R_RegisterCCallable("redatam4r", "bin_get_rvector", (DL_FUNC)&bin_get_rvector);
	R_RegisterCCallable("redatam4r", "pck_get_rvector", (DL_FUNC)&pck_get_rvector);
	R_RegisterCCallable("redatam4r", "chr_get_rvector", (DL_FUNC)&chr_get_rvector);
	R_RegisterCCallable("redatam4r", "int_get_rvector", (DL_FUNC)&int_get_rvector);
	R_RegisterCCallable("redatam4r", "lng_get_rvector", (DL_FUNC)&lng_get_rvector);
	R_RegisterCCallable("redatam4r", "real_get_rvector", (DL_FUNC)&real_get_rvector);
	/*R_useDynamicSymbols(dll, FALSE);
	R_forceSymbols(dll, TRUE);*/
}

}
