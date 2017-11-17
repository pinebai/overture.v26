*
* Make a 3d valve
*
create mappings
  * main cylinder
  Cylinder
    mappingName
      outerCylinder
    * orient the cylinder so y-axis uis axial direction
    orientation
      2 0 1
    bounds on the radial variable
      .4 1.
    bounds on the axial variable
      -.1 .5
    lines
      55 15 9 55 11 9
    boundary conditions
      -1 -1 0 3 0 2
    share
      0 0 0 1 0 2 
  exit
* core of the main cylinder
  Box
    mappingName
      cylinderCore
    set corners
    -.5 .5  0. .5  -.5 .5
    lines
      19 17 19
    boundary conditions
      0 0 1 2 0 0 
    share
      0 0 3 1 0 0
  exit
* valve stem  
  Cylinder
    mappingName
      valveStem
    * orient the cylinder so y-axis uis axial direction
    orientation
      2 0 1
    bounds on the radial variable
      .2 .6
    bounds on the axial variable
      -.5 -.2
    lines
      41 9 9
    boundary conditions
      -1 -1 3 2 2 0 
    share
      0 0 4 3 0 0 
    exit
*  Make a 2d cross-section of the valve
  SmoothedPolygon
    mappingName
      valveCrossSection
    vertices
      4
      .4 0.
      .85 0.
      .65 -.2
      .4 -.2
    n-dist
    fixed normal distance
      .15
    lines
      65 17
    sharpness
      30 
      30 
      30
      30
    boundary conditions
      0 0 1 0
  exit
*  Make the valve as a body of revolution
  body of revolution
    mappingName
    valve
    choose a point on the line to revolve about
      0. 1. 0.
    lines
      41 11 35
    boundary conditions
      0 0 2 0 -1 -1
    share
      0 0 3 0 0 0
  exit
* 2D cross section for the stopper
  SmoothedPolygon
    mappingName
      stopperCrossSection
    vertices
      4
      .65 -.5
      .65 -.3
      .85 -.1
      1. -.1
    n-dist
      fixed normal distance
      .15
  exit
* stopper
  body of revolution
    mappingName
      stopper
    choose a point on the line to revolve about
      0. 1. 0.
    boundary conditions
      1 1 2 0 -1 -1
    share
      4 2 0 0 0 0
    lines
      35 11 41
  exit
  view mappings
    outerCylinder
    cylinderCore
    valveStem
    valve
    stopper
  exit
exit
* 
generate an overlapping grid
    cylinderCore
    outerCylinder
    stopper
    valve
    valveStem
  done 
  change parameters
    cell centering
      cell centered for all grids
    exit
  pause
  compute overlap
exit
save an overlapping grid
valve3dCC.hdf
valve3d
exit


