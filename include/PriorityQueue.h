#ifndef __OV_PRIORITY_QUEUE__
#define __OV_PRIORITY_QUEUE__


template<class T> class PriorityQueueTemplate;
template<class T> class PriorityQueueTemplateiterator;
template<class T> class PriorityQueueTemplate_constiterator;

template<typename T> 
class PriorityQueueTemplateiterator
{
  friend class PriorityQueueTemplate<T>;
  friend class PriorityQueueTemplate_constiterator<T>;
public:
  PriorityQueueTemplateiterator () 
  { }

  PriorityQueueTemplateiterator ( const typename list<T>::iterator & it ) 
  { listIter = it; }

  PriorityQueueTemplateiterator ( const PriorityQueueTemplateiterator<T> &it ) 
  { listIter = it.listIter; }

  PriorityQueueTemplateiterator & operator++() 
  { listIter++; return *this; }
  
  PriorityQueueTemplateiterator operator++(int) 
  { return PriorityQueueTemplateiterator<T>(listIter++); }

  T &operator*() 
  { return *listIter; }

  const T &operator*() const 
  { return *listIter; }

  bool operator==(const PriorityQueueTemplateiterator<T> &i) const 
  { return listIter == i.listIter; }

  bool operator!=(const PriorityQueueTemplateiterator<T> &i) const 
  { return listIter != i.listIter; }

private:
  typename list<T>::iterator listIter;

};

template<typename T> 
class PriorityQueueTemplate_constiterator
{
  friend class PriorityQueueTemplate<T>;
  friend class PriorityQueueTemplateiterator<T>;

public:
  PriorityQueueTemplate_constiterator () 
  { }

  PriorityQueueTemplate_constiterator ( const typename list<T>::const_iterator & it ) 
  { listIter = it; }

  PriorityQueueTemplate_constiterator ( const PriorityQueueTemplate_constiterator<T> &it ) 
  { listIter = it.listIter; }

  PriorityQueueTemplate_constiterator & operator++()  
  { listIter++; return *this; }

  PriorityQueueTemplate_constiterator operator++(int) 
  { return PriorityQueueTemplate_constiterator<T>(listIter++); }

  const T &operator*() const 
  { return *listIter; }

  bool operator==(const PriorityQueueTemplate_constiterator<T> &i) const 
  { return listIter == i.listIter; }

  bool operator!=(const PriorityQueueTemplate_constiterator<T> &i) const 
  { return listIter != i.listIter; }

private:
  typename list<T>::const_iterator listIter;

};

template<class T>
class PriorityQueueTemplate
{
public:
  PriorityQueueTemplate() { }
  ~PriorityQueueTemplate() { }

  typedef PriorityQueueTemplateiterator<T> iterator;
  typedef PriorityQueueTemplate_constiterator<T> const_iterator;

  void erase(typename PriorityQueueTemplate<T>::iterator &i) 
  { theQueue.erase(i.listIter); }

  typename PriorityQueueTemplate<T>::iterator insert(T& data, double priority) 
  { theQueue.push_back(data); return PriorityQueueTemplateiterator<T>((--theQueue.end()));}

  typename PriorityQueueTemplate<T>::iterator begin() 
  { return PriorityQueueTemplateiterator<T>(theQueue.begin()); }

  typename PriorityQueueTemplate<T>::iterator end() 
  { return PriorityQueueTemplateiterator<T>(theQueue.end()); }

#if 0
  PriorityQueueTemplate<T>::const_iterator insert(T& data, double priority) const 
  { theQueue.push_back(T); return PriorityQueueTemplate_constiterator<T>(--(theQueue.end()));}
#endif

  typename PriorityQueueTemplate<T>::const_iterator begin() const 
  { return PriorityQueueTemplate_constiterator<T>(theQueue.begin()); }

  typename PriorityQueueTemplate<T>::const_iterator end() const 
  { return PriorityQueueTemplate_constiterator<T>(theQueue.end()); }

  void clear() 
  { theQueue.clear(); }

  bool empty() const 
  { return theQueue.empty() ; }

  int size() const
  { return theQueue.size(); }

private:
  list<T> theQueue;
};


//
// __ov_batchQueue_struct is a utility structure for the priority batch queue
// it is used to maintin both the batch itself and this priority bounds
//
template<class T>
struct __ov_batchQueue_struct
{
  PriorityQueueTemplate<T> queue;
  real priorityBegin;
  real priorityEnd;
};

template<class T> class PriorityBatchQueue;
template<class T> class PriorityBatchQueueIterator;
template<class T> class PriorityBatchQueueConstIterator;

//
// iterator for a PriorityBatchQueue
//
template<class T>
class PriorityBatchQueueIterator
{
  friend class PriorityBatchQueue<T>;

public:
  PriorityBatchQueueIterator () 
  { refQueue=NULL; }

  PriorityBatchQueueIterator ( const PriorityBatchQueueIterator<T> &it_ ) 
  { refQueue = it_.refQueue; batch=it_.batch; it = it_.it; } 

  PriorityBatchQueueIterator & operator++() 
  { 
    if ( refQueue==NULL ) throw "cannot increment uninitialized iterator";
    if ( batch!=refQueue->end() )
      {
	if ( it != (*batch).queue.end() )
	  it++;
	else
	  {
	    batch++;
	    if ( batch!=refQueue->end() ) it = (*batch).queue.begin();
	  }
	if ( batch!=refQueue->end() )
	  if ( it == (*batch).queue.end() ) ++(*this);
      }

    return *this;
  }

  PriorityBatchQueueIterator & operator=(const PriorityBatchQueueIterator<T> &it_ ) 
  { refQueue = it_.refQueue; batch=it_.batch; it = it_.it; return *this;} 
  PriorityBatchQueueIterator & operator=(const PriorityBatchQueueConstIterator<T> &it_ ) 
  { refQueue = it_.refQueue; batch=it_.batch; it = it_.it; return *this;} 

  PriorityBatchQueueIterator operator++(int) 
  { 
    PriorityBatchQueueIterator oldIt = *this;

    ++(*this);
#if 0
    if ( refQueue==NULL ) throw "cannot increment uninitialized iterator";
    if ( batch!=refQueue->end() )
      if ( it != (*batch).queue.end() )
	it++;
      else
	{
	  batch++;
	  if ( batch!=refQueue->end() ) it = (*batch).queue.begin();
	}
#endif
    return oldIt;
  }
    
  T &operator*() 
  { return *it; }

  const T &operator*() const 
  { return *it; }

  bool operator==(const PriorityBatchQueueIterator<T> &i) const 
  { 
    bool areEqual = true;
    areEqual = (refQueue == i.refQueue);
    if ( areEqual && refQueue!=NULL )
      {
	if ( (areEqual = (batch == i.batch)) ) 
	  if ( areEqual && batch!=refQueue->end() ) areEqual = (it==i.it);
      }
    
    return areEqual;
  }

  bool operator!=(const PriorityBatchQueueIterator<T> &i) const 
  { 
    return ! ( (*this)==i );
  }
    

private:

  list< __ov_batchQueue_struct<T> > *refQueue;
  typename list< __ov_batchQueue_struct<T> >::iterator batch;
  typename PriorityQueueTemplate<T>::iterator it;

};

//
// const_iterator for a PriorityBatchQueue
//
template<class T>
class PriorityBatchQueueConstIterator
{
  friend class PriorityBatchQueue<T>;

public:
  PriorityBatchQueueConstIterator () 
  { refQueue=NULL; }

  PriorityBatchQueueConstIterator ( const PriorityBatchQueueConstIterator<T> &it_ ) 
  { refQueue = it_.refQueue; batch=it_.batch; it = it_.it; } 

  PriorityBatchQueueConstIterator & operator=(const PriorityBatchQueueConstIterator<T> &it_ ) 
  { refQueue = it_.refQueue; batch=it_.batch; it = it_.it; return *this; } 


  PriorityBatchQueueConstIterator & operator++() 
  { 
    if ( refQueue==NULL ) throw "cannot increment uninitialized iterator";
    if ( batch!=refQueue->end() )
      {
	if ( it != (*batch).queue.end() )
	  it++;
	else
	  {
	    batch++;
	    if ( batch!=refQueue->end() ) it = (*batch).queue.begin();
	  }
	if ( batch!=refQueue->end() )
	  if ( it==(*batch).queue.end() ) ++(*this);
      }
    return *this;
  }

  PriorityBatchQueueConstIterator operator++(int) 
  { 
    PriorityBatchQueueConstIterator oldIt = *this;

    ++(*this);
#if 0
    if ( refQueue==NULL ) throw "cannot increment uninitialized iterator";
    if ( batch!=refQueue->end() )
      if ( it != (*batch).queue.end() )
	it++;
      else
	{
	  batch++;
	  if ( batch!=refQueue->end() ) it = (*batch).queue.begin();
	}
#endif

    return oldIt;
  }
    
  T &operator*() 
  { return (T&)(*it); }

  const T &operator*() const 
  { return *it; }

  bool operator==(const PriorityBatchQueueConstIterator<T> &i) const 
  { 
    bool areEqual = true;
    areEqual = (refQueue == i.refQueue);
    if ( areEqual && refQueue!=NULL )
      {
	if ( (areEqual = (batch == i.batch)) ) 
	  if ( areEqual && batch!=refQueue->end() ) areEqual = (it==i.it);
      }
    
    return areEqual;
  }

  bool operator!=(const PriorityBatchQueueConstIterator<T> &i) const 
  { 
    return ! ( (*this)==i );
  }
    
private:
  list< __ov_batchQueue_struct<T> > *refQueue;
  typename list< __ov_batchQueue_struct<T> >::const_iterator batch;
  typename PriorityQueueTemplate<T>::const_iterator it;
};

//
// A priority batch queue.
// this class encapsulates a queue where items are inserted with a particular
// priority but are placed in batches of similar priority.  Iterations through
// the queue start at the highest priority batch iterate through it, and then
// continue with the next batch and so on.  Within a batch, items are stuck
// in a FIFO queue encapsulated by PriorityQueueTemplate (above).
//
template<class T>
class PriorityBatchQueue 
{
  friend class PriorityBatchQueueIterator<T>;
  friend class PriorityBatchQueueConstIterator<T>;

public:
  PriorityBatchQueue( real priorityBoundPercent_=.25 ) : priorityBoundPercent(priorityBoundPercent_), maxP(-REAL_MAX), minP(REAL_MAX) { }
  PriorityBatchQueue( const PriorityBatchQueue<T> &pQ ) : queues(pQ.queues), priorityBoundPercent(priorityBoundPercent), maxP(-REAL_MAX), minP(REAL_MAX) { }
  ~PriorityBatchQueue() { }

  typedef PriorityBatchQueueIterator<T> iterator;
  typedef PriorityBatchQueueConstIterator<T> const_iterator;

  void erase(typename PriorityBatchQueue<T>::iterator &i) 
  { 
    (*(i.batch)).queue.erase(i.it); 
    if ((*(i.batch)).queue.empty()) queues.erase(i.batch); 
    i.refQueue = 0;
  }

  double maxPriority() const
  {
    return maxP;
  }

  double minPriority() const
  {
    return minP;
  }

  typename PriorityBatchQueue<T>::iterator reprioritize(typename PriorityBatchQueue<T>::iterator &i, double newPriority)
  {
    T  data = *i;
    typename PriorityBatchQueue<T>::iterator retiter = i;
    ++retiter;
    erase(i);
    i = insert(data, newPriority);

    return retiter;
  }

  typename PriorityBatchQueue<T>::iterator insert(T& data, double priority) 
  { 

    minP = min((double)minP,priority);
    maxP = max((double)maxP,priority);

    //cout<<"inserting with priority "<<priority<<endl;
    typename list< __ov_batchQueue_struct<T> >::iterator insBatch=queues.begin();

    typename PriorityQueueTemplate<T>::iterator i;

    if ( insBatch!=queues.end() )
      {
	while ( insBatch!=queues.end() )
	  {
	    if ( priority>(*insBatch).priorityBegin || 
		 ( priority<=(*insBatch).priorityBegin && priority>(*insBatch).priorityEnd ) )
	      break;

	    insBatch++;
	  }
      }

    real pa = (1.+ priorityBoundPercent)*priority;
    real pb = (1.- priorityBoundPercent)*priority;

    bool createNew = insBatch==queues.end() ? true : ( priority>(*insBatch).priorityBegin ? true : false );

    //if ( insBatch!=queues.end() ) cout<<"priority , priorityBegin "<<priority<<" "<<(*insBatch).priorityBegin<<endl;

    if ( createNew )
      {
	__ov_batchQueue_struct<T> newQStruct;
	newQStruct.priorityBegin = max(pa,pb);
	newQStruct.priorityEnd = (insBatch==queues.end()) ? 
	  min(pa,pb) : min(min(pa,pb),(*insBatch).priorityBegin);
	queues.insert( insBatch, newQStruct );
	insBatch--;
	i = (*insBatch).queue.insert(data,priority);
      }
    else //if ( priority>=(*insBatch).priorityBegin && priority<(*insBatch).priorityEnd )
      {
	i = (*insBatch).queue.insert(data,priority);
      }

    typename PriorityBatchQueue<T>::iterator newIter;
    newIter.refQueue = (list< __ov_batchQueue_struct<T> > *) &queues;
    newIter.batch = insBatch;
    newIter.it = i;

    //    cout<<"batch queue size "<<queues.size()<<endl;
    return newIter;
  }

  typename PriorityBatchQueue<T>::iterator begin() 
  { 
    typename PriorityBatchQueue<T>::iterator ib;
    ib.batch = queues.begin();
    ib.refQueue = &queues;
    if (queues.begin()!=queues.end()) ib.it = (*(ib.batch)).queue.begin();
    return ib;
  }

  typename PriorityBatchQueue<T>::iterator end() 
  { 
    typename PriorityBatchQueue<T>::iterator ie;
    ie.batch = queues.end();
    ie.refQueue = &queues;
    return ie;
  }

#if 0
  PriorityBatchQueue<T>::const_iterator insert(T& data, double priority) const 
  { 
  }
#endif

  typename PriorityBatchQueue<T>::const_iterator begin() const 
  { 
    typename PriorityBatchQueue<T>::const_iterator ib;
    ib.batch = queues.begin();
    ib.refQueue = ( list< __ov_batchQueue_struct<T> > * )&queues;
    if (queues.begin()!=queues.end()) ib.it = (*(ib.batch)).queue.begin();
    return ib;
  }

  typename PriorityBatchQueue<T>::const_iterator end() const 
  { 
    typename PriorityBatchQueue<T>::const_iterator ie;
    ie.batch = queues.end();
    ie.refQueue = ( list< __ov_batchQueue_struct<T> > * )&queues;
    return ie;
  }

  void clear() 
  { 
    for ( typename list< __ov_batchQueue_struct<T> >::iterator i=queues.begin(); i!=queues.end(); i++ ) (*i).queue.clear();
    queues.clear(); 
  }

  bool empty() const 
  { return queues.empty(); }

  int size() const 
  { 
    int s=0;
    for ( typename list< __ov_batchQueue_struct<T> >::const_iterator i=queues.begin(); i!=queues.end(); i++ )
      s+=(*i).queue.size();
    return s;
  }

  int numberOfBatches() const 
  { return queues.size(); }

private:
  list< __ov_batchQueue_struct<T> > queues;
  real priorityBoundPercent;
  real maxP;
  real minP;
};

#endif
