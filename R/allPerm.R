allPerm = function(mcObj){
    if(class(mcObj) != "mc")
      stop("mcObject must be of class mc")
    
    numPerms = multinom(mcObj$set)
    result = rep(0, mcObj$length * numPerms)
    
    r = .C("all", id = as.integer(mcObj$id), result = as.integer(result))

    if(mcObj$mode == "integer"){
        return(matrix(r$result, ncol = mcObj$length, byrow = TRUE))
    }else{
      return(matrix(mcObj$elements[r$result], ncol = mcObj$length, byrow = TRUE))
    }
}
