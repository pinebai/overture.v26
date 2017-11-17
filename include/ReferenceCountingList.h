#ifndef REFERENCE_COUNTING_LIST_H
#define REFERENCE_COUNTING_LIST_H

class ReferenceCounting;

/// \brief class to store the pointers to the ReferenceCounting items in the LinkedList.
class ReferenceCountingItem  
{
 public:
  ReferenceCounting* val;    
  ReferenceCountingItem* next;
  int id;
  
  ReferenceCountingItem(){};
  ReferenceCountingItem( ReferenceCounting* value, const int & identifier=-1 ){ val=value; id=identifier; next=NULL; }
  ~ReferenceCountingItem(){}
};
  
/// \brief Linked List of ReferenceCounting items.
class ReferenceCountingList   
{
 public:
  ReferenceCountingItem *start;
  ReferenceCountingItem *end;
  ReferenceCountingList();
  ~ReferenceCountingList();
  void add( ReferenceCounting *val, const int & identifier=-1 );
  int remove( ReferenceCounting *val );
  ReferenceCounting *find( int identifier ) const;
  
};

#endif
