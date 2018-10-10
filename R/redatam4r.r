
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)

documentation <- function(x, ...) UseMethod("documentation")

documentation.redatam.entity <- function(x) attr(x, "documentation")
documentation.redatam.variable.int <- function(x) attr(x, "documentation")
documentation.redatam.variable.lng <- function(x) attr(x, "documentation")
documentation.redatam.variable.chr <- function(x) attr(x, "documentation")
documentation.redatam.variable.bin <- function(x) attr(x, "documentation")
documentation.redatam.variable.dbl <- function(x) attr(x, "documentation")
documentation.redatam.variable.pck <- function(x) attr(x, "documentation")


description <- function(x, ...) UseMethod("description")

description.redatam.entity <- function(x) attr(x, "description")
description.redatam.variable.int <- function(x) attr(x, "description")
description.redatam.variable.lng <- function(x) attr(x, "description")
description.redatam.variable.chr <- function(x) attr(x, "description")
description.redatam.variable.bin <- function(x) attr(x, "description")
description.redatam.variable.dbl <- function(x) attr(x, "description")
description.redatam.variable.pck <- function(x) attr(x, "description")

missing.value <- function(x, ...) UseMethod("missing.value")

missing.value.redatam.variable.int <- function(x) attr(x, "missing.value")
missing.value.redatam.variable.lng <- function(x) attr(x, "missing.value")
missing.value.redatam.variable.chr <- function(x) attr(x, "missing.value")
missing.value.redatam.variable.bin <- function(x) attr(x, "missing.value")
missing.value.redatam.variable.dbl <- function(x) attr(x, "missing.value")
missing.value.redatam.variable.pck <- function(x) attr(x, "missing.value")

notapplicable.value <- function(x, ...) UseMethod("notapplicable.value")

notapplicable.value.redatam.variable.int <- function(x) attr(x, "notapplicable.value")
notapplicable.value.redatam.variable.lng <- function(x) attr(x, "notapplicable.value")
notapplicable.value.redatam.variable.chr <- function(x) attr(x, "notapplicable.value")
notapplicable.value.redatam.variable.bin <- function(x) attr(x, "notapplicable.value")
notapplicable.value.redatam.variable.dbl <- function(x) attr(x, "notapplicable.value")
notapplicable.value.redatam.variable.pck <- function(x) attr(x, "notapplicable.value")

binpck_indexing <- function(x, i) {
	if (is.logical(i)) {
		i = which(i)
	}
	if (!is.numeric(i) || !is.integer(i)) {
		stop('invalid indexing mode')
	}
	.Call('binpck_get', x, i)
}

'[.redatam.variable.pck' <- function(x, i) binpck_indexing(x, i)
'[.redatam.variable.bin' <- function(x, i) binpck_indexing(x, i)
length.redatam.variable.pck <- function(x) attr(x, 'instance.num')
length.redatam.variable.bin <- function(x) attr(x, 'instance.num')
