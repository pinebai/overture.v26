// Bill's useful definitions
#ifndef WDHDEFS_H
#define WDHDEFS_H "wdhdefs.h"

#ifndef NO_APP
#include "OvertureDefine.h"
#include <A++.h>
#endif

#ifndef NO_APP
#include "aString.H"
#else
#include "GUIDefine.h"
#include <string>
#ifndef aString
#define aString std::string
#endif
#endif

#include "GUITypes.h"

extern int debug;     // variable used for debugging

const int axis1 = 0;  // names for the 3 parameter axes
const int axis2 = 1;
const int axis3 = 2;

const int xAxis = 0;  // names for the 3 cartesian coordinate directions
const int yAxis = 1;
const int zAxis = 2;

const int Start = 0;  // names for left/right top/bottom back/front
const int End   = 1;


#define sprintf2(a,b) (sprintf(a,b),a)
#define sprintf3(a,b,c) (sprintf(a,b,c),a)
#define sprintf4(a,b,c,d) (sprintf(a,b,c,d),a)
#define sprintf5(a,b,c,d,e) (sprintf(a,b,c,d,e),a)
#define sprintf6(a,b,c,d,e,f) (sprintf(a,b,c,d,e,f),a)
#define sprintf7(a,b,c,d,e,f,g) (sprintf(a,b,c,d,e,f,g),a)
#define sprintf8(a,b,c,d,e,f,g,h) (sprintf(a,b,c,d,e,f,g,h),a)
#define sprintf9(a,b,c,d,e,f,g,h,i) (sprintf(a,b,c,d,e,f,g,h,i),a)

// sprintf like routine that returns the formated string s
extern aString sPrintF(const char *format, ...);
extern char* sPrintF(char *s, const char *format, ...);
extern aString& sPrintF(aString & s, const char *format, ...);

extern int 
sScanF(const aString & s, const char *format, 
       void *p0,
       void *p1=NULL, 
       void *p2=NULL, 
       void *p3=NULL,
       void *p4=NULL,
       void *p5=NULL,
       void *p6=NULL,
       void *p7=NULL,
       void *p8=NULL,
       void *p9=NULL,
       void *p10=NULL,
       void *p11=NULL,
       void *p12=NULL,
       void *p13=NULL,
       void *p14=NULL,
       void *p15=NULL,
       void *p16=NULL,
       void *p17=NULL,
       void *p18=NULL,
       void *p19=NULL,
       void *p20=NULL,
       void *p21=NULL,
       void *p22=NULL,
       void *p23=NULL,
       void *p24=NULL,
       void *p25=NULL,
       void *p26=NULL,
       void *p27=NULL,
       void *p28=NULL,
       void *p29=NULL );

extern int 
fScanF(FILE *file, const char *format, 
       void *p0,
       void *p1=NULL, 
       void *p2=NULL, 
       void *p3=NULL,
       void *p4=NULL,
       void *p5=NULL,
       void *p6=NULL,
       void *p7=NULL,
       void *p8=NULL,
       void *p9=NULL,
       void *p10=NULL,
       void *p11=NULL,
       void *p12=NULL,
       void *p13=NULL,
       void *p14=NULL,
       void *p15=NULL,
       void *p16=NULL,
       void *p17=NULL,
       void *p18=NULL,
       void *p19=NULL,
       void *p20=NULL,
       void *p21=NULL,
       void *p22=NULL,
       void *p23=NULL,
       void *p24=NULL,
       void *p25=NULL,
       void *p26=NULL,
       void *p27=NULL,
       void *p28=NULL,
       void *p29=NULL
  );


//kkc #ifndef NO_APP 
// These versions only print on processor 0 
void printF(const char *format, ...);
void fPrintF(FILE *file, const char *format, ...);
//kkc #endif

//--------------------------------------------------------------------------------------
//  Read a line from standard input
//   char s[]   : char array in which to store the line
//   lim        : maximum number of chars that can be saved in s
//-------------------------------------------------------------------------------------
int getLine( char s[], int lim);
int getLine( aString &answer );

// conversion routine used by sscanf to convert %e to %le and %f to %lf when OV_USE_DOUBLE is defined
aString ftor(const char *s);

// *wdh* 100109 GUITypes::real getCPU();
// inline this for performance 
#define SECOND EXTERN_C_NAME(second)
extern "C" { 
  void SECOND( real & time );
}

inline GUITypes::real getCPU()
{
  #ifndef USE_PPP
    real time;
    SECOND( time );
    return time;
  #else
    #ifdef OV_USE_DOUBLE
      return MPI_Wtime(); 
    #else
      return (real)MPI_Wtime(); 
    #endif
  #endif
}


// **** here are global variables that are initialized in initOvertureGlobalVariables()****
extern const GUITypes::real Pi;
extern const GUITypes::real twoPi;

extern const aString nullString;                     // null string for default arguments
extern const aString blankString;                   // blank string for default arguments
#ifndef NO_APP
extern const Index nullIndex;
extern const Range nullRange;
extern const Range faceRange;
#endif

int
helpOverture( const aString & documentPrefix, const aString & label );

#ifndef NO_APP

#ifndef USE_PPP
#define GET_NUMBER_OF_ARRAYS Array_Domain_Type::getNumberOfArraysInUse()
#else
#define GET_NUMBER_OF_ARRAYS 0
#endif

#endif

// ------------------------------------------------------------------------------------------
// ----------- Macros that access the serial array from a P++ parallel array  ---------------
// ------------------------------------------------------------------------------------------
#ifdef USE_PPP

#define OV_GET_SERIAL_ARRAY(TYPE,U,ULOCAL) TYPE ## SerialArray ULOCAL; getLocalArrayWithGhostBoundaries(U,ULOCAL);
#define OV_GET_SERIAL_ARRAY_CONST(TYPE,U,ULOCAL) TYPE ## SerialArray ULOCAL; getLocalArrayWithGhostBoundaries(U,ULOCAL);
#define OV_GET_SERIAL_ARRAY_CONDITIONAL(TYPE,U,ULOCAL,CONDITION) \
TYPE ## SerialArray ULOCAL; if( CONDITION ) getLocalArrayWithGhostBoundaries(U,ULOCAL);

#else

#define OV_GET_SERIAL_ARRAY(TYPE,U,ULOCAL) TYPE ## SerialArray & ULOCAL = U;
#define OV_GET_SERIAL_ARRAY_CONST(TYPE,U,ULOCAL) const TYPE ## SerialArray & ULOCAL = U;

#define OV_GET_SERIAL_ARRAY_CONDITIONAL(TYPE,U,ULOCAL,CONDITION) TYPE ## SerialArray & ULOCAL = U;

#endif


#endif // WDHDEFS_H
