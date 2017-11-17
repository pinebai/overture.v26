#ifndef GENERICDATABASE_H 
#define GENERICDATABASE_H "GenericDataBase.h"

#include "OvertureDefine.h"

// *wdh* for pgi
#ifdef __PGI
 #include "math.h"
 inline double abs(double x){ return fabs(x); }
 #include <cmath> 
#endif


#if !defined(OV_BOOL_DEFINED) && !defined(USE_PPP) 
  typedef int bool;    // this will be the new standard for Boolean
#endif

#ifdef OV_EXCEPTIONS_NOT_SUPPORTED
  #define throw exit(1); cout << 
#endif

#include "A++.h"
#include "aString.H"

class ReferenceCountingList;

//--------------------------------------------------------------------------
//
//  GenericDataBase: 
//    This is a base class to support access to and from a data-base.
//    This class knows how to get and put the types
//       o int, float, double, aString
//    as well as A++ arrays
//       o intArray, floatArray, doubleArray
//    as well as
//       o "c" arrays of Strings.
//
//--------------------------------------------------------------------------

class GenericDataBase
{
public:
  aString className;  // this should be the name of the derived class
                     // check this to be safe before casting to the derived class
  aString OvertureVersion; // holds version number used when file was created

  enum InputOutputMode
  {
    normalMode=0,        // normal mode
    streamInputMode,     // stream mode (objects are input/output as a stream, no directories, no names)
    streamOutputMode,
    noStreamMode,        // do not use stream mode 
    bufferMode      // only used internally
  };
  
  enum ParallelIOModeEnum
  {
    independentIO,   // use H5FD_MPIO_INDEPENDENT for mpi IO
    collectiveIO,    // use H5FD_MPIO_COLLECTIVE  for mpi IO
    multipleFileIO   // use multiple files with serial IO
  };

public:
  GenericDataBase();
  GenericDataBase(const GenericDataBase &);
  virtual ~GenericDataBase();
  virtual GenericDataBase* virtualConstructor() const;
  virtual GenericDataBase & operator=(const GenericDataBase & );

  // open a data-base file
  virtual int mount(const aString & fileName, const aString & flags);
  // flush data, close the file
  virtual int unmount();
  // flush the data to the file 
  virtual int flush();
  
  virtual int isNull() const;

  virtual int turnOnWarnings();
  virtual int turnOffWarnings();

  // create a sub-directory
  virtual int create( GenericDataBase & db, const aString & name, const aString & dirClassName );
  // find a sub-directory (crash if not found)
  virtual int find(GenericDataBase & db, const aString & name, const aString & dirClassName ) const;

  // locate a sub-directory
  virtual int locate(GenericDataBase & db, const aString & name, const aString & dirClassName ) const;

  // find the names of all objects with a given className
  virtual int find(aString *name, const aString & dirClassName, 
                      const int & maxNumber, int & actualNumber) const;
  // find all sub-directories (and their names) with a given dirClassName
  virtual int find(GenericDataBase *db, aString *name, const aString & dirClassName, 
                      const int & maxNumber, int & actualNumber) const;

  ReferenceCountingList* getList() const;  // holds a list of reference counted objects that are in the data base
  virtual int getID() const;  // get the identifier for this directory

  // build a directory with the given ID
  virtual int build(GenericDataBase & db, int id);

  // remove a directory
  // virtual int destroy(const aString & name, const aString & flags);

  // put a float/int/double/aString
  virtual int put( const float & x, const aString & name );
  virtual int put( const double & x, const aString & name );
  virtual int put( const int & x, const aString & name );
  virtual int put( const aString & x, const aString & name );
#ifdef OV_BOOL_DEFINED
  virtual int put( const bool & x, const aString & name );
#endif

  // get a float/int/double/aString
  virtual int get( float & x, const aString & name ) const;
  virtual int get( double & x, const aString & name ) const;
  virtual int get( int & x, const aString & name ) const;
  virtual int get( aString & x, const aString & name ) const;
#ifdef OV_BOOL_DEFINED
  virtual int get( bool & x, const aString & name ) const;
#endif

  // put a float/int/double A++ array
  virtual int put( const floatSerialArray & x, const aString & name );
  virtual int put( const doubleSerialArray & x, const aString & name );
  virtual int put( const intSerialArray & x, const aString & name );

  virtual int putDistributed( const floatArray & x, const aString & name );
  virtual int putDistributed( const doubleArray & x, const aString & name );
  virtual int putDistributed( const intArray & x, const aString & name );

  // get a float/int/double A++ array
  virtual int get( floatSerialArray & x, const aString & name, Index *Iv=NULL ) const;
  virtual int get( doubleSerialArray & x, const aString & name, Index *Iv=NULL ) const;
  virtual int get( intSerialArray & x, const aString & name, Index *Iv=NULL ) const;

  virtual int getDistributed( floatArray & x, const aString & name ) const;
  virtual int getDistributed( doubleArray & x, const aString & name ) const;
  virtual int getDistributed( intArray & x, const aString & name ) const;


  // put/get a "c" array of float/int/double/Strings 
  virtual int put( const int    x[],const aString & name, const int number ); 
  virtual int put( const float  x[],const aString & name, const int number ); 
  virtual int put( const double x[],const aString & name, const int number ); 
  virtual int put( const aString x[],const aString & name, const int number ); 

  virtual int get( int    x[], const aString & name, const int number ) const;
  virtual int get( float  x[], const aString & name, const int number ) const;
  virtual int get( double x[], const aString & name, const int number ) const;
  virtual int get( aString x[], const aString & name, const int number ) const;

  virtual void setMode(const InputOutputMode & mode=normalMode);
  virtual InputOutputMode getMode() const;

  // set the parallel write and read modes.
  static void setParallelWriteMode(ParallelIOModeEnum mode);
  static void setParallelReadMode(ParallelIOModeEnum mode);

  // get the parallel write and read modes.
  static ParallelIOModeEnum getParallelWriteMode();
  static ParallelIOModeEnum getParallelReadMode();


  // return the number of additional local files that are written for distributed data
  int getNumberOfLocalFilesForReading() const;
  // return the number of additional local files that are read for distributed data
  int getNumberOfLocalFilesForWriting() const;

  static int setMaximumNumberOfFilesForWriting( int maxNumberOfFiles );

  // output statistics on the file 
  virtual void printStatistics() const;

 protected:
  InputOutputMode mode;
  int issueWarnings;
  ReferenceCountingList *referenceCountingList;  // holds a list of reference counted objects that are in the data base
  static ParallelIOModeEnum parallelReadMode,parallelWriteMode;

  int numberOfLocalFilesForWriting, numberOfLocalFilesForReading;  // number of local files for parallel IO
  int numberOfProcessorsUsedToWriteFile;

  static int maximumNumberOfLocalFilesForWriting;

};


#endif
