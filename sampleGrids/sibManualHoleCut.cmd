*
* command file to create a sphere in a box
*
*
create mappings
* first make a sphere
Sphere
exit
*
* now make a mapping for the north pole
*
reparameterize
  orthographic
    specify sa,sb
      2.5 2.5
  exit
  lines
    23 23 7   12 12 4
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    north-pole
exit
*
* now make a mapping for the south pole
*
reparameterize
  orthographic
    choose north or south pole
      -1
    specify sa,sb
      2.5 2.5
  exit
  lines
    23 23 7  12 12 4 
  boundary conditions
    0 0 0 0 1 0
  share
    0 0 0 0 1 0
  mappingName
    south-pole
exit
*
* Here is the box
*
Box
  set corners
    -2 2 -2 2 -2 2
  lines
    33 33 33  17 17 17 
  mappingName
    box
  exit
exit
*
generate an overlapping grid
  box
  north-pole
  south-pole
  done
  change parameters
    * interpolation type
    *   explicit for all grids
*    turn on phantom hole cutting 
    phantom hole cutting
      north-pole
       box
      south-pole
       box
    done
*    cut a hole in the box
    manual hole cutting
      box
        16 18 16 18 16 18 
      done
    ghost points
      all
      2 2 2 2 2 2
  exit
  display intermediate results
  compute overlap
  continue


exit
save an overlapping grid
sib2.hdf
sib2
exit
