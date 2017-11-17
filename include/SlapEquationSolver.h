#ifndef SLAP_EQUATION_SOLVER_H
#define SLAP_EQUATION_SOLVER_H

//
//  Slap solver for Oges
// 


//kkc 040415 #include <iostream.h>
#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#include <math.h>
#include <assert.h>

#include "EquationSolver.h"


class SlapEquationSolver : public EquationSolver
{
 public:
  SlapEquationSolver(Oges & oges_);
  virtual ~SlapEquationSolver();

  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f);

 virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 

 protected:
  int solve();
  int allocateWorkSpace();
  
  int solverJob;
  int ndiwk;
  IntegerArray iwk;
  int ndwk;
  RealArray wk;
  real fillinRatio;
  
};


#endif
