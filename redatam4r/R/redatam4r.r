#' Read a Redatam database
#'
#' @param dic_path Path to the .dic file of the database
#' @useDynLib redatam4r read_redatam
#' @export
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)

#' Describe a Redatam object (database, entity or variable), as seen by libredatam
#'
#' @param obj Object to describe
#' @export
description <- function(x, ...) UseMethod("description")

#' @export
description.redatam.database     <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.entity       <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.int <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.lng <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.chr <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.bin <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.dbl <- function(x) cat(attr(x, "description"))

#' @export
description.redatam.variable.pck <- function(x) cat(attr(x, "description"))


variable_indexing <- function(x, i, funcname) {
	if (is.logical(i)) {
		i <- which(i);
	}
	i <- as.integer(i);
	negindx = all(i <= 0)
	posindx = any(i >= 0)
	if (negindx && posindx) {
		stop('only 0\'s may be mixed with negative subscripts')
	}
	if (negindx) {
		i = (1:length(x))[i]
	}
	i = i[i != 0]
	.Call(funcname, x, as.integer(i-1))
}

#' Index a Redatam BIN variable. Triggers a RBF file read. Note that the returned object size may be much greater than the size of the RBF file.
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns an integer vector (which can have factors) with the elements from the RBF file.
#' @useDynLib redatam4r bin_get_rvector
#' @export
'[.redatam.variable.bin'  <- function(x, i) variable_indexing(x, i, 'bin_get_rvector')

#' Index a Redatam PCK variable. Triggers a RBF file read. Note that the returned object size may be much greater than the size of the RBF file.
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns an integer vector (which can have factors) with the elements from the RBF file.
#' @useDynLib redatam4r pck_get_rvector
#' @export
'[.redatam.variable.pck'  <- function(x, i) variable_indexing(x, i, 'pck_get_rvector')

#' Index a Redatam CHR variable. Triggers a RBF file read
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns a character vector with the elements from the RBF file.
#' @useDynLib redatam4r chr_get_rvector
#' @export
'[.redatam.variable.chr'  <- function(x, i) variable_indexing(x, i, 'chr_get_rvector')

#' Index a Redatam INT variable. Triggers a RBF file read
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns an integer vector (which can have factors) with the elements from the RBF file.
#' @useDynLib redatam4r int_get_rvector
#' @export
'[.redatam.variable.int'  <- function(x, i) variable_indexing(x, i, 'int_get_rvector')


#' Index a Redatam LNG variable. Triggers a RBF file read
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns an integer vector (which can have factors) with the elements from the RBF file.
#' @useDynLib redatam4r lng_get_rvector
#' @export
'[.redatam.variable.lng'  <- function(x, i) variable_indexing(x, i, 'lng_get_rvector')


#' Index a Redatam DBL variable. Triggers a RBF file read
#'
#' @param x Object to index
#' @param i Indices (can be numeric, integer or boolean)
#' @return Returns a numeric vector with the elements from the RBF file.
#' @useDynLib redatam4r real_get_rvector
#' @export
'[.redatam.variable.real' <- function(x, i) variable_indexing(x, i, 'real_get_rvector')

assign_indexing <- function(x, i, val) stop('writing to redatam objects is not allowed')

#' @export
'[<-.redatam.database'      <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.entity'        <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.bin'  <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.pck'  <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.chr'  <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.int'  <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.lng'  <- function(x, i, v) assign_indexing(x, i, v)

#' @export
'[<-.redatam.variable.real' <- function(x, i, v) assign_indexing(x, i, v)


#' @export
length.redatam.variable.bin  <- function(x) attr(x, 'instance.num')

#' @export
length.redatam.variable.pck  <- function(x) attr(x, 'instance.num')

#' @export
length.redatam.variable.chr  <- function(x) attr(x, 'instance.num')

#' @export
length.redatam.variable.int  <- function(x) attr(x, 'instance.num')

#' @export
length.redatam.variable.lng  <- function(x) attr(x, 'instance.num')

#' @export
length.redatam.variable.real <- function(x) attr(x, 'instance.num')


#' @export
print.redatam.variable.bin  <- function(x, ...) print(x[1:length(x)], ...)

#' @export
print.redatam.variable.pck  <- function(x, ...) print(x[1:length(x)], ...)

#' @export
print.redatam.variable.chr  <- function(x, ...) print(x[1:length(x)], ...)

#' @export
print.redatam.variable.int  <- function(x, ...) print(x[1:length(x)], ...)

#' @export
print.redatam.variable.lng  <- function(x, ...) print(x[1:length(x)], ...)

#' @export
print.redatam.variable.real <- function(x, ...) print(x[1:length(x)], ...)


#' @export
format.redatam.variable.bin  <- function(x, ...) format(x[1:length(x)], ...)

#' @export
format.redatam.variable.pck  <- function(x, ...) format(x[1:length(x)], ...)

#' @export
format.redatam.variable.chr  <- function(x, ...) format(x[1:length(x)], ...)

#' @export
format.redatam.variable.int  <- function(x, ...) format(x[1:length(x)], ...)

#' @export
format.redatam.variable.lng  <- function(x, ...) format(x[1:length(x)], ...)

#' @export
format.redatam.variable.real <- function(x, ...) format(x[1:length(x)], ...)

