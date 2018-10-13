
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)

description <- function(x, ...) UseMethod("description")

description.redatam.database     <- function(x) cat(attr(x, "description"))
description.redatam.entity       <- function(x) cat(attr(x, "description"))
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

'[.redatam.variable.bin'  <- function(x, i) variable_indexing(x, i, 'bin_get_rvector')
'[.redatam.variable.pck'  <- function(x, i) variable_indexing(x, i, 'pck_get_rvector')
'[.redatam.variable.chr'  <- function(x, i) variable_indexing(x, i, 'chr_get_rvector')
'[.redatam.variable.int'  <- function(x, i) variable_indexing(x, i, 'int_get_rvector')
'[.redatam.variable.lng'  <- function(x, i) variable_indexing(x, i, 'lng_get_rvector')
'[.redatam.variable.real' <- function(x, i) variable_indexing(x, i, 'real_get_rvector')

assign_indexing <- function(x, i, val) stop('writing to redatam objects is not allowed')
'[<-.redatam.database'      <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.entity'        <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.bin'  <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.pck'  <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.chr'  <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.int'  <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.lng'  <- function(x, i, v) assign_indexing(x, i, v)
'[<-.redatam.variable.real' <- function(x, i, v) assign_indexing(x, i, v)

length.redatam.variable.bin  <- function(x) attr(x, 'instance.num')
length.redatam.variable.pck  <- function(x) attr(x, 'instance.num')
length.redatam.variable.chr  <- function(x) attr(x, 'instance.num')
length.redatam.variable.int  <- function(x) attr(x, 'instance.num')
length.redatam.variable.lng  <- function(x) attr(x, 'instance.num')
length.redatam.variable.real <- function(x) attr(x, 'instance.num')

print.redatam.variable.bin  <- function(x, ...) print(x[1:length(x)], ...)
print.redatam.variable.pck  <- function(x, ...) print(x[1:length(x)], ...)
print.redatam.variable.chr  <- function(x, ...) print(x[1:length(x)], ...)
print.redatam.variable.int  <- function(x, ...) print(x[1:length(x)], ...)
print.redatam.variable.lng  <- function(x, ...) print(x[1:length(x)], ...)
print.redatam.variable.real <- function(x, ...) print(x[1:length(x)], ...)

format.redatam.variable.bin  <- function(x, ...) format(x[1:length(x)], ...)
format.redatam.variable.pck  <- function(x, ...) format(x[1:length(x)], ...)
format.redatam.variable.chr  <- function(x, ...) format(x[1:length(x)], ...)
format.redatam.variable.int  <- function(x, ...) format(x[1:length(x)], ...)
format.redatam.variable.lng  <- function(x, ...) format(x[1:length(x)], ...)
format.redatam.variable.real <- function(x, ...) format(x[1:length(x)], ...)

