#ifndef LISTOFDOUBLEGRIDCOLLECTIONFUNCTION_H
#define LISTOFDOUBLEGRIDCOLLECTIONFUNCTION_H "ListOfDoubleGridCollectionFunction.h"
// *wdh* for pgi: 110529
#ifdef __PGI
 #include "GenericDataBase.h"
#endif
class doubleGridCollectionFunction;
#include "OvertureTypes.h"
#include "ReferenceCounting.h"

//==========================================================================
//	ListOfDoubleGridCollectionFunction Class Template
//
//  This is a template list class for holding items that are reference
//  counted in the style of A++ arrays. In particular, any class doubleGridCollectionFunction to
//  be used with this list must have the following characteristics:
//
//  There must be member functions:
//          doubleGridCollectionFunction::reference( const doubleGridCollectionFunction & t ) 
//          doubleGridCollectionFunction::breakReference()
//  that behave in the same way as A++ arrays.
//  In addition the assignment operator "=" must be deep copy.
//
//  This class keeps an array of pointers to objects of class doubleGridCollectionFunction. 
//  The array is dynamic so that only 
//  memory and time limit the number of objects stored on the list.
//  
// Reference Counting
//  This class supports two types of reference counting. The suggested
//  way to do reference counting is to use the reference and breakReference
//  member functions as shown in the examples below. If instead you want
//  to keep pointers to ListOfReferenceObjects then you can use the
//  incrementReferenceCount(), decrementReferenceCount() and
//  getReferenceCount() member functions (derived from the ReferenceCounting
//  in order to mange references). It is up to you to call delete when
//  the reference count reaches zero.
//
//  Typical Usage:
//
//     floatArray a(10),b(5);
//     ListOfDoubleGridCollectionFunction<floatArray> list,list2;
//     list.addElement();      // add an element
//     list[0].reference(a);   // reference to array a
//     list.addElement(b);     // add element and reference to b in one step
//     list[1]=b;
//     list[0]=1.;             //  same as a=1.;
//
//     list2.reference(list);  // list2 and list are now the same
//     list2[0]=2.;            // same as list[0]=2.;
//     list2.breakReference();
//     list2[0]=5.;            // does not change list[0]
//
//
// Authors: Jeff Saltzman and Bill Henshaw
//
//==========================================================================

class ListOfDoubleGridCollectionFunction : public ReferenceCounting
{
    
  public:

  //  Default constructor.
  ListOfDoubleGridCollectionFunction();			

  // Create a list with a given number of elements
  ListOfDoubleGridCollectionFunction( const int numberOfElements );

  //   Copy constructor, deep copy by default
  ListOfDoubleGridCollectionFunction(const ListOfDoubleGridCollectionFunction&, 
    const CopyType=DEEP  ); 

  ~ListOfDoubleGridCollectionFunction();			//   Destructor.
    
  // Assignment operator (deep copy)
  ListOfDoubleGridCollectionFunction& operator=(const ListOfDoubleGridCollectionFunction&);

  // ---------List Management Functions-------------

  void addElement( const int index );        // Add an object to the list

  void addElement();                         // Add an object to the end of the list

  void addElement( const doubleGridCollectionFunction & t, const int index ); // Add an object and reference to object t

  void addElement( const doubleGridCollectionFunction & t );                  // Add an object to the end, and reference to object t

  int getLength() const                      // Get length of list.
           {return rcData->listLength;};    

  doubleGridCollectionFunction& operator[](const int index) const;	     // Reference the object at a given location.

  void deleteElement( const doubleGridCollectionFunction & X );		     // Find an element on the list and delete it.

  void deleteElement( const int index );     // Delete the element at the given index

  void deleteElement();                      // Delete the element appearing last in the list

  void destroy();                            // Destroy the list

  void swapElements( const int i, const int j );  // Swap two elements (for sorting)
					       
  int getIndex(const doubleGridCollectionFunction & X) const;

  // reference one list to another
  void reference( const ListOfDoubleGridCollectionFunction & list );

  // break any references with this list
  void breakReference();

 protected:

  void initialize();  
  void deleteStuff();

  // create space so a new element can be inserted
  void openAPositionForAnElement( const int index );

  void checkRange( const int ) const;	// Internal range check function.

// private:
 public:
  int listLength() const
           {return rcData->listLength;};    // Get length of list.
  
   class RCData : public ReferenceCounting  // holds reference counted data except A++ arrays
   {
     friend class ListOfDoubleGridCollectionFunction;
     RCData(){ }     
     ~RCData(){ }
     int listLength;	// Number of elements in the list.
     int memAlloc;	// Current amount of memory allocated for the list.
     doubleGridCollectionFunction **aList;		// Pointer to a list of pointers to object doubleGridCollectionFunction.
   };
   
  RCData *rcData;

 private:
//
// Virtual member functions used only through class ReferenceCounting:
//
  virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=( (ListOfDoubleGridCollectionFunction &)x); }
  virtual void reference( const ReferenceCounting& x)
    { reference( (ListOfDoubleGridCollectionFunction &) x); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new ListOfDoubleGridCollectionFunction(*this, ct); }
};

#endif
