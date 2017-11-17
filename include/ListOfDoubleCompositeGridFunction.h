#ifndef LISTOFDOUBLECOMPOSITEGRIDFUNCTION_H
#define LISTOFDOUBLECOMPOSITEGRIDFUNCTION_H "ListOfDoubleCompositeGridFunction.h"
// *wdh* for pgi: 110529
#ifdef __PGI
 #include "GenericDataBase.h"
#endif
class doubleCompositeGridFunction;
#include "OvertureTypes.h"
#include "ReferenceCounting.h"

//==========================================================================
//	ListOfDoubleCompositeGridFunction Class Template
//
//  This is a template list class for holding items that are reference
//  counted in the style of A++ arrays. In particular, any class doubleCompositeGridFunction to
//  be used with this list must have the following characteristics:
//
//  There must be member functions:
//          doubleCompositeGridFunction::reference( const doubleCompositeGridFunction & t ) 
//          doubleCompositeGridFunction::breakReference()
//  that behave in the same way as A++ arrays.
//  In addition the assignment operator "=" must be deep copy.
//
//  This class keeps an array of pointers to objects of class doubleCompositeGridFunction. 
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
//     ListOfDoubleCompositeGridFunction<floatArray> list,list2;
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

class ListOfDoubleCompositeGridFunction : public ReferenceCounting
{
    
  public:

  //  Default constructor.
  ListOfDoubleCompositeGridFunction();			

  // Create a list with a given number of elements
  ListOfDoubleCompositeGridFunction( const int numberOfElements );

  //   Copy constructor, deep copy by default
  ListOfDoubleCompositeGridFunction(const ListOfDoubleCompositeGridFunction&, 
    const CopyType=DEEP  ); 

  ~ListOfDoubleCompositeGridFunction();			//   Destructor.
    
  // Assignment operator (deep copy)
  ListOfDoubleCompositeGridFunction& operator=(const ListOfDoubleCompositeGridFunction&);

  // ---------List Management Functions-------------

  void addElement( const int index );        // Add an object to the list

  void addElement();                         // Add an object to the end of the list

  void addElement( const doubleCompositeGridFunction & t, const int index ); // Add an object and reference to object t

  void addElement( const doubleCompositeGridFunction & t );                  // Add an object to the end, and reference to object t

  int getLength() const                      // Get length of list.
           {return rcData->listLength;};    

  doubleCompositeGridFunction& operator[](const int index) const;	     // Reference the object at a given location.

  void deleteElement( const doubleCompositeGridFunction & X );		     // Find an element on the list and delete it.

  void deleteElement( const int index );     // Delete the element at the given index

  void deleteElement();                      // Delete the element appearing last in the list

  void destroy();                            // Destroy the list

  void swapElements( const int i, const int j );  // Swap two elements (for sorting)
					       
  int getIndex(const doubleCompositeGridFunction & X) const;

  // reference one list to another
  void reference( const ListOfDoubleCompositeGridFunction & list );

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
     friend class ListOfDoubleCompositeGridFunction;
     RCData(){ }     
     ~RCData(){ }
     int listLength;	// Number of elements in the list.
     int memAlloc;	// Current amount of memory allocated for the list.
     doubleCompositeGridFunction **aList;		// Pointer to a list of pointers to object doubleCompositeGridFunction.
   };
   
  RCData *rcData;

 private:
//
// Virtual member functions used only through class ReferenceCounting:
//
  virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=( (ListOfDoubleCompositeGridFunction &)x); }
  virtual void reference( const ReferenceCounting& x)
    { reference( (ListOfDoubleCompositeGridFunction &) x); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new ListOfDoubleCompositeGridFunction(*this, ct); }
};

#endif
