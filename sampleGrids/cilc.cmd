*
* circle in a long channel with a stretched grid 
*
create mappings
  rectangle
    set corners
    -2.5  7.5  -2.5 2.5
    lines
      101 51
    boundary conditions
      1 1 1 1
    mappingName
    square
    exit
  Annulus
    inner radius
      .5
    outer radius
      1.25
    lines
      85 17
    boundary conditions
    -1 -1 1 0
    exit
  * stretch the annulus *********
  *
  * Stretch coordinates
  stretch coordinates
    transform which mapping?
    Annulus
    stretch
      specify stretching along axis=1
        layers
        1
        1. 9. 0.
        * 1. 20. 0.  ** for testing
        exit
      exit
    mappingName
    annulus
    exit
  *
  exit
  generate an overlapping grid
    square
    annulus
    done
    change parameters
      * interpolation type
      *  explicit for all grids
      ghost points
        all
        2 2 2 2 2 2
      exit
    compute overlap
    exit
  save an overlapping grid
  cilc.hdf
  cilc
  exit

