
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)

description <- function(x, ...) UseMethod("description")

description.redatam.entity <- function(x) cat(attr(x, "description"))
description.redatam.variable.int <- function(x) cat(attr(x, "description"))
description.redatam.variable.lng <- function(x) cat(attr(x, "description"))
description.redatam.variable.chr <- function(x) cat(attr(x, "description"))
description.redatam.variable.bin <- function(x) cat(attr(x, "description"))
description.redatam.variable.dbl <- function(x) cat(attr(x, "description"))
description.redatam.variable.pck <- function(x) cat(attr(x, "description"))

variable_indexing <- function(x, i, funcname) {
	if (is.logical(i)) {
		i = which(i)
	}
	if (!is.numeric(i) && !is.integer(i)) {
		stop('invalid indexing mode')
	}
	.Call(funcname, x, i)
}

'[.redatam.variable.bin' <- function(x, i) variable_indexing(x, i, 'bin_get_rvector')
'[.redatam.variable.pck' <- function(x, i) variable_indexing(x, i, 'pck_get_rvector')
'[.redatam.variable.chr' <- function(x, i) variable_indexing(x, i, 'chr_get_rvector')
'[.redatam.variable.int' <- function(x, i) variable_indexing(x, i, 'int_get_rvector')
'[.redatam.variable.lng' <- function(x, i) variable_indexing(x, i, 'lng_get_rvector')
'[.redatam.variable.real' <- function(x, i) variable_indexing(x, i, 'real_get_rvector')

length.redatam.variable.bin <- function(x) attr(x, 'instance.num')
length.redatam.variable.pck <- function(x) attr(x, 'instance.num')
length.redatam.variable.chr <- function(x) attr(x, 'instance.num')
length.redatam.variable.int <- function(x) attr(x, 'instance.num')
length.redatam.variable.lng <- function(x) attr(x, 'instance.num')
length.redatam.variable.real <- function(x) attr(x, 'instance.num')

#print.redatam.variable.pck <- function(x) .Call('binpck_print_integer', x)
#print.redatam.variable.bin <- function(x) .Call('binpck_print_integer', x)

#format.redatam.variable.pck <- function(x, trim=F, digits=NULL, nsmall=0L,  na.encode=F, justify=c('left', 'right', 'centre', 'none')) {
#	.Call('binpck_format_integer', x)
#}
#format.redatam.variable.bin <- function(x, ...) {
#	format.redatam.variable.pck(x, ...)
#}
