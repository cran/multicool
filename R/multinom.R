multinom = function(x, counts = FALSE){
  
  u = NULL
  nu = -1
  
  if(!counts){
    u = as.vector(table(x))
    nu = length(u)
  }else{
    ## make sure x is a vector of counts
    is.wholenumber = function(x, tol = .Machine$double.eps^0.5){abs(x - round(x)) < tol}
    
    if(any(!is.wholenumber(x)) || any(x < 0))
      stop("if counts == TRUE then all elements of x must be integer and >= 0")
    
    u = x
    nu = length(x)
  }
  
  result = -1
  
  r = .C("multinomCoeff", u = as.integer(u),
          nu = as.integer(nu), result = as.integer(result))
  
  return(r$result)
}