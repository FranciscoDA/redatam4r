
#' useDynLib redatam4r read_redatam
read.redatam <- function(dic_path) .Call("read_redatam", dic_path)