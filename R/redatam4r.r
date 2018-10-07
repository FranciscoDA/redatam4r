
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)

documentation <- function(x, ...) UseMethod("documentation")

documentation.redatam.entity <- function(x) attr(x, "documentation")

description <- function(x, ...) UseMethod("description")

description.redatam.entity <- function(x) attr(x, "description")
