#ifndef MAPPINGS_FROM_CAD_H
#define MAPPINGS_FROM_CAD_H

// This class is used to build Overture Mapping's from CAD files.

#include "MappingRC.h"
//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "NurbsMapping.h"

class IgesReader;

// ===================================================================================================
/// \brief This class is used to build Overture Mapping's from CAD files.
// ===================================================================================================
class 
MappingsFromCAD : public ReferenceCounting
{
public:

  MappingsFromCAD();
  ~MappingsFromCAD();

  int 
  readMappings(MappingInformation & mapInfo, aString fName = nullString, bool chooseAll = FALSE);
  
  void 
  fileContents(aString fileName, IgesReader * &iges_, int & numberOfNurbs, int & numberOfFiniteElements,
	       int & numberOfNodes, int & status );

  CompositeSurface * 
  readSomeNurbs( MappingInformation & mapInfo, IgesReader *iges_, int startMap, int endMap, 
		 int numberOfNurbs, int & status );// numberOfNurbs should be stored in the IgesReader class

  static int getTransformationMatrix(const int & item, IgesReader & iges, RealArray & rotation, RealArray & translation);

protected:

  enum LineTypes
  {
    general=0,
    horizontal,
    vertical
  };

  enum BoundaryTypes
  {
    generalBoundary=0,
    simpleReparameterization,
    coons13LeftRight,                  // coons patch formed from curves 1 and 3
    coons13BottomTop,                  // coons patch formed from curves 1 and 3
    coons24LeftRight,                  // coons patch formed from curves 2 and 4
    coons24BottomTop,                  // coons patch formed from curves 2 and 4
    coons                     // coons patch formed from 4 curves
  }  boundaryType;

  
  int createCompositeCurve(const int & item, 
			   IgesReader & iges, 
                           int & maximumNumberOfSubCurves,
			   int & numberOfSubCurves, 
			   Mapping ** & mapPointer,
			   Mapping *surf,
			   RealArray & surfaceParameterScale,bool scaleTheCurve=true,
                           bool expectPhysicalSpaceCurve=false );
  
  int createCurveOnAParametricSurface(const int & item, 
				      IgesReader & iges, 
				      Mapping *surf, 
				      Mapping *&mapPointer,
                                      int & maximumNumberOfSubCurves,
				      Mapping ** & subCurve,
				      RealArray & surfaceParameterScale);
  

  int 
  createIgesReader( GenericGraphicsInterface & gi, aString & fileName, IgesReader * &iges, FILE * &fp,
		    bool useGivenFileName = FALSE);

  int createSurface(int surface, 
		    IgesReader & iges, 
		    Mapping *&mapPointer,
		    RealArray & surfaceParameterScale = Overture::nullRealArray() );

  int createTrimmedSurface(const int & item, 
			   IgesReader & iges, 
			   Mapping *&mapPointer);
  
  int isUntrimmedSurface(IgesReader & iges, int item );

  int readFiniteElements(IgesReader & iges );


  int readOneCurve(int curve,
		   IgesReader & iges, 
		   Mapping * & mapPointer,
		   RealArray & curveParameterScale);
  
  int scaleCurve( Mapping & mapping, Mapping & surf, RealArray & surfaceParameterScale);
  
  int createBoundaryEntity(const int & item, 
			   IgesReader & iges, 
			   const int & untrimmedSurfSeq,
			   NurbsMapping *&mapPointer,
			   RealArray & curveParameterScale);

  int readOneCurveAsNURBS(int curve,
			  IgesReader &iges,
			  NurbsMapping *&mapPointer,
			  RealArray & curveParameterScale);

  int createBoundedSurface(int entity, IgesReader &iges, Mapping *&mapPointer);

  int createManifoldSolidBRepObject(int entity, IgesReader &iges, 
                                    MappingInformation & mapInfo,  IntegerArray & mapNumber,
                                    int & numberOfMapsCreated, int & visibleItem);

  IgesReader *igesPointer;
  MappingInformation *mapInfoPointer;
  GraphicsParameters params;


  // kkc 12/10/01 the following 2 variables were made dynamic 
  //  int lineType[20]; // for an error on dec-linux-compaq
  //real subCurveBound[2][20]; // bounds on sub curves

  int  *lineType;
  real *subCurveBound[2];

  real curveBound[2][2];     //  bounds on composite curve

  int revCount, revCurveCount, axisLineCount; 

};


#endif

