*
create mappings
  Box
    lines
      129 129 129 
    mappingName
      box
  exit
exit
*
generate an overlapping grid
  box
  done
  change parameters
    ghost points
      all
      2 2 2 2 2 2
    order of accuracy
     fourth order
  exit
  compute overlap
exit
save an overlapping grid
box128.order4.hdf
box
exit
