#include "redatam4r.h"
#include "rindexing.h"
#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>

extern "C" {
static const R_CMethodDef CEntries[] = {
	{"_redatam4r_read_redatam",     (DL_FUNC) &_redatam4r_read_redatam, 1},
	{"_redatam4r_bin_get_rvector",  (DL_FUNC) &_redatam4r_bin_get_rvector, 2},
	{"_redatam4r_pck_get_rvector",  (DL_FUNC) &_redatam4r_pck_get_rvector, 2},
	{"_redatam4r_chr_get_rvector",  (DL_FUNC) &_redatam4r_chr_get_rvector, 2},
	{"_redatam4r_int_get_rvector",  (DL_FUNC) &_redatam4r_int_get_rvector, 2},
	{"_redatam4r_lng_get_rvector",  (DL_FUNC) &_redatam4r_lng_get_rvector, 2},
	{"_redatam4r_real_get_rvector", (DL_FUNC) &_redatam4r_real_get_rvector, 2},
	{NULL, NULL, 0}
};

void R_init_redatam4r(DllInfo* dll) {
	R_RegisterCCallable("redatam4r", "_redatam4r_read_redatam",     (DL_FUNC)&_redatam4r_read_redatam);
	R_RegisterCCallable("redatam4r", "_redatam4r_bin_get_rvector",  (DL_FUNC)&_redatam4r_bin_get_rvector);
	R_RegisterCCallable("redatam4r", "_redatam4r_pck_get_rvector",  (DL_FUNC)&_redatam4r_pck_get_rvector);
	R_RegisterCCallable("redatam4r", "_redatam4r_chr_get_rvector",  (DL_FUNC)&_redatam4r_chr_get_rvector);
	R_RegisterCCallable("redatam4r", "_redatam4r_int_get_rvector",  (DL_FUNC)&_redatam4r_int_get_rvector);
	R_RegisterCCallable("redatam4r", "_redatam4r_lng_get_rvector",  (DL_FUNC)&_redatam4r_lng_get_rvector);
	R_RegisterCCallable("redatam4r", "_redatam4r_real_get_rvector", (DL_FUNC)&_redatam4r_real_get_rvector);
	/*R_useDynamicSymbols(dll, FALSE);
	R_forceSymbols(dll, TRUE);*/
}

}
