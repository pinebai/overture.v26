*
* 3D Cylinder in a box
*
*
* usage: ogen [noplot] cylBoxArg -factor=<num> -order=[2/4/6/8] -interp=[e/i] -name= -xa= -xb= -ya= -yb= -za= -zb=
* 
* examples:
*     ogen noplot cylBoxArg -factor=2 -order=2 -interp=e 
*     ogen noplot cylBoxArg -factor=4 -order=2 -interp=e 
*     ogen noplot cylBoxArg -factor=4 -order=2 -interp=e -xb=1.5 -name="cylBox2eLong.hdf
*     ogen noplot cylBoxArg -factor=2 -order=2 -interp=e -za=0. -zb=2. -name="cylBox2eZ2.hdf"
* 
* -- set default parameter values:
$innerRad=.25; 
$xa=-.75; $xb=.75; $ya=-.75; $yb=.75; $za=-.5; $zb=.5;
$order=2; $factor=1; $interp="i"; $name="";
$orderOfAccuracy = "second order"; $ng=2; $interpType = "implicit for all grids";
* 
* get command line arguments
GetOptions("order=i"=>\$order,"factor=i"=> \$factor,"interp=s"=> \$interp,"xa=f"=> \$xa,"xb=f"=> \$xb,"ya=f"=> \$ya,"yb=f"=> \$yb,"za=f"=> \$za,"zb=f"=> \$zb,"name=s"=>\$name);
* 
if( $order eq 4 ){ $orderOfAccuracy="fourth order"; $ng=2; }\
elsif( $order eq 6 ){ $orderOfAccuracy="sixth order"; $ng=4; }\
elsif( $order eq 8 ){ $orderOfAccuracy="eighth order"; $ng=6; }
if( $interp eq "e" ){ $interpType = "explicit for all grids"; }
* 
$suffix = ".order$order"; 
if( $name eq "" ){ $name = "cylBox" . "$interp$factor" . $suffix . ".hdf"; }
* 
$ds=.1/$factor;
*
*
* Make a cylinder in a box
*
create mappings
  Box
    mappingName
      box
  set corners
  lines
    set corners
      $xa $xb $ya $yb $za $zb
    lines
      $nx = int( ($xb-$xa)/$ds + 1.5 );
      $ny = int( ($yb-$ya)/$ds + 1.5 );
      $nz = int( ($zb-$za)/$ds + 1.5 );
      $nx $ny $nz
    boundary conditions
      1 1 2 2 3 4 
    share
      0 0 0 0 1 2
  exit
*
  Cylinder
    mappingName
      cylinder
    bounds on the radial variable
      $deltaRad=4*$ds; 
      $outerRad=$innerRad+$deltaRad; 
      $innerRad $outerRad
    bounds on the axial variable
      $za $zb
    lines
      $nt = int( 2.*3.1415*($innerRad+$outerRad)*.5/$ds + 1.5 );
      $nr = int( $deltaRad/$ds + 2.5 );
      $nt $nz $nr 
    boundary conditions
     -1 -1 3 4 5 0  
    share
      0 0 1 2 0 0
  exit
exit
*
*
generate an overlapping grid
    box
    cylinder
  done
  change parameters
    ghost points
      all
      $ng $ng $ng $ng $ng $ng 
    order of accuracy
      $orderOfAccuracy
    interpolation type
      $interpType
  exit
*  display intermediate results
* pause
  compute overlap
  exit
*
save an overlapping grid
$name
cylBox
exit


