/*
** This software is copyright (C) by the Lawrence Berkeley National
** Laboratory.  Permission is granted to reproduce this software for
** non-commercial purposes provided that this notice is left intact.
**  
** It is acknowledged that the U.S. Government has rights to this
** software under Contract DE-AC03-765F00098 between the U.S.  Department
** of Energy and the University of California.
**  
** This software is provided as a professional and academic contribution
** for joint exchange. Thus it is experimental, is provided ``as is'',
** with no warranties of any kind whatsoever, no support, no promise of
** updates, or printed documentation. By using this software, you
** acknowledge that the Lawrence Berkeley National Laboratory and Regents
** of the University of California shall have no liability with respect
** to the infringement of other copyrights by any part of this software.
**  
** For further information about this software, contact:
** 
**         Dr. John Bell
**         Bldg. 50D, Rm. 129,
**         Lawrence Berkeley National Laboratory
**         Berkeley, CA, 94720
**         jbbell@lbl.gov
*/

#ifndef _POINTERS_H_
#define _POINTERS_H_

// *wdh*
#include "OvertureDefine.h"

//
// $Id: Pointers.H,v 1.5 2005/10/29 17:20:06 henshaw Exp $
//

#include <BL_Assert.H>
#include <Boolean.H>
#include <BoxLib.H>
#include <UseCount.H>

//
//@Man:
//@Memo: A Smart Pointer for Intrinsic or User-Defined Types
/*@Doc:

  The template class CpPtr<T> provides a simple wrapper around a pointer
  to type T (T*) that builds a copy of the pointed-to object when copied
  from one CpPtr<T> to another.  This is in contrast to a reference-counted
  pointer class that would maintain one pointed-to object with a reference
  count indicating the number of references.  Hence we call this a
  "copied" smart pointer class.  It is intended for use with any type
  type T, including the intrinsic types.  This class does not supply
  an operator->(), as such an operator on intrinsic types has only recently
  become a part of the C++ language, and many compilers do not yet implement
  it.
*/

template <class T>
class CpPtr
{
public:
    //
    //@ManDoc: The default constructor.  The wrapped pointer is null.
    //
    inline CpPtr ();
    //
    //@ManDoc: Construct a CpPtr<T> setting the wrapped pointer to rhs.
    //
    inline CpPtr (T* rhs);
    //
    //@ManDoc: The destructor.  Deletes the wrapped pointer.
    //
    inline ~CpPtr ();

    /*@ManDoc: The copy constructor.  If the pointer wrapped by rhs is null,
               the wrapped pointer is null here as well.  Otherwise,
               the contained pointer here is set to a new'd copy of that
               wrapped by rhs, with the two pointed-to values being identical.
               This assumes that type T has a well-defined and accessible copy
               constructor.  T must also be a concrete type, not a abstract
               type.
    */
    inline CpPtr (const CpPtr<T>& rhs);

    /*@ManDoc: Sets the wrapped pointer to rhs.  Deletes the previously
               wrapped pointer.
    */
    inline CpPtr<T>& operator= (T* rhs);

    /*@ManDoc: The copy assignment operator.  If the pointer wrapped by rhs
               is null, the wrapped pointer is null here as well.  Otherwise,
               the contained pointer here is set to a new'd copy of that
               wrapped by rhs, with the two pointed-to values being identical.
               This assumes that type T has a well-defined and accessible copy
               constructor.  T must also be a concrete type, not a abstract
               type.
    */
    inline CpPtr<T>& operator= (const CpPtr<T>& rhs);

    /*@ManDoc: Returns a reference to the value pointed to by the wrapped
               pointer; i.e. dereferencing this CpPtr<T>, returns the
               dereferenced wrapped pointer.  It is an error if the wrapped
               pointer is null.
    */
    inline T& operator* () const;
    //
    //@ManDoc: Returns true if the wrapped pointer null.
    //
    inline bool isNull () const;
    //
    //@ManDoc: Sets the wrapped pointer to null and returns the previous value.
    //
    inline T* release ();
    //
    //@ManDoc: Are the two pointers (not the values to which they point) equal?
    //
    inline bool operator== (const CpPtr<T>& rhs) const;
    //
    //@ManDoc: Are the two pointers not equal?
    //
    inline bool operator!= (const CpPtr<T>& rhs) const;

protected:
    T* ptr;
};

//
//@Man:
//@Memo: A Smart Pointer for User-Defined Types
/*@Doc:

  The template class CpClassPtr<T> is derived from CpPtr<T>.  It provides a
  simple wrapper around a pointer to type T (a T*) that "does the right thing"
  when copied from one CpPtr<T> to another.  The type T MUST be a user-defined
  type, not an intrinsic type.  Given this restriction, we can supply
  an operator->().
*/

template<class T>
class CpClassPtr :
    public CpPtr<T>
{
public:
    //
    //@ManDoc: The default constructor.  The wrapped pointer is null.
    //
    inline CpClassPtr ();
    //
    //@ManDoc: Construct a CpPtr<T> setting the wrapped pointer to rhs.
    //
    inline CpClassPtr (T* rhs);

    /*@ManDoc: The copy constructor.  If the pointer wrapped by rhs is null,
               the wrapped pointer is null here as well.  Otherwise,
               the contained pointer here is set to a new'd copy of that
               wrapped by rhs, with the two pointed-to values being identical.
               This assumes that type T has a well-defined and accessible copy
               constructor.  T must also be a concrete type, not a abstract
               type.
    */
    inline CpClassPtr (const CpClassPtr<T>& rhs);

    /*@ManDoc: Sets the wrapped pointer to rhs.  Deletes the previously
               wrapped pointer.
    */
    inline CpClassPtr<T>& operator= (T* rhs);

    /*@ManDoc: The copy assignment operator.  If the pointer wrapped by rhs
               is null, the wrapped pointer is null here as well.  Otherwise,
               the contained pointer here is set to a new'd copy of that
               wrapped by rhs, with the two pointed-to values being identical.
               This assumes that type T has a well-defined and accessible copy
               constructor.  T must also be a concrete type, not a abstract
               type.
    */
    inline CpClassPtr<T>& operator= (const CpClassPtr<T>& rhs);
    //
    //@ManDoc: Applies operator-> to the wrapped pointer.
    //
    inline T* operator-> () const;
};

//
//@Man:
//@Memo: A Reference Counted Smart Pointer for Intrinsic or User-Defined Types
/*@Doc:

  The template class LnPtr<T> provides a reference counted wrapper around a
  pointer to type T (a T*).  This "smart" pointer is intended for use with
  any type type T, including the intrinsic types.  For this reason, we do
  not supply an operator->(), as such an operator on intrinsic types has only
  recently become a part of the C++ language and many compilers do not yet
  implement it.
*/

template<class T>
class LnPtr
{
public:
    //
    //@ManDoc: The default constructor.  The wrapped pointer is null.
    //
    inline LnPtr ();
    //
    //@ManDoc: Construct a LnPtr<T> setting the wrapped pointer to rhs.
    //
    inline LnPtr (T* rhs);

    /*@ManDoc: The copy assignment operator.  The contained pointer is set
               to the one wrapped by rhs.  The reference count is decremented
               on this object and the reference count is incremented for
               the newly wrapped pointer.
    */
    inline LnPtr<T>& operator= (const LnPtr<T>& rhs);

    /*@ManDoc: Sets the wrapped pointer to rhs.  Decrements the count
               on the previously wrapped pointer and deletes it if there
               was only one reference.
    */
    LnPtr<T>& operator= (T* rhs)
    {
        //
        // This is inlined here as OCC won't have it any other way :-(
        //
        if (unique())
            delete this->ptr;
        this->ptr = rhs;
        ucnt = UseCount();
        return *this;
    }

    /*@ManDoc: The destructor -- decrements the reference count and deletes
               the wrapped pointer if there is only one reference.
    */
    inline ~LnPtr ();
    //
    //@ManDoc: Returns true if only one reference to the wrapped pointer.
    //
    inline bool unique () const;
    //
    //@ManDoc: Returns the number of references to the wrapped pointer.
    //
    inline int linkCount () const;

    /*@ManDoc: Returns a reference to the value pointed to by the wrapped
               pointer; i.e. dereferencing this LnPtr<T>, returns the
               dereferenced wrapped pointer.  It is an error if the wrapped
               pointer is null.
    */
    inline T& operator* () const;
    //
    //@ManDoc: Returns true if the wrapped pointer is null.
    //
    inline bool isNull () const;
    //
    //@ManDoc: Are the two pointers (not the values to which they point) equal?
    //
    inline bool operator== (const LnPtr<T>& rhs) const;
    //
    //@ManDoc: Are the two pointers not equal?
    //
    inline bool operator!= (const LnPtr<T>& rhs) const;

protected:
    T*       ptr;

private:
    UseCount ucnt;
};

//
//@Man:
//@Memo: A Smart Reference Counted Pointer for User-Defined Types
/*@Doc:

  The template class LnClassPtr<T> is derived from LnPtr<T>.  It provides a
  reference counted wrapper around a pointer to type T (a T*).  The type T
  MUST be a user-defined type, not an intrinsic type.  Given this
  restriction, we can supply an operator->().
*/

template<class T>
class LnClassPtr
    : public LnPtr<T>
{
public:
    //
    //@ManDoc: The default constructor.  The wrapped pointer is null.
    //
    inline LnClassPtr ();
    //
    //@ManDoc: Construct a LnPtr<T> setting the wrapped pointer to rhs.
    //
    inline LnClassPtr (T* rhs);

    /*@ManDoc: The copy assignment operator.  The contained pointer is set
               to the one wrapped by rhs.  The reference count is decremented
               on this object and the reference count is incremented for
               the newly wrapped pointer.
    */
    inline LnClassPtr<T>& operator= (const LnClassPtr<T>& rhs);

    /*@ManDoc: Sets the wrapped pointer to rhs.  Decrements the count
               on the previously wrapped pointer and deletes it if there
               was only one reference.
    */
    inline LnClassPtr<T>& operator= (T* rhs);
    //
    //@ManDoc: Applies operator-> to the wrapped pointer.
    //
    inline T* operator->() const;
};

//
// This stuff is commented out for now.  If someone really uses it,
// we'll uncomment it, as well as document it.
//

#if 0

//
// Object Factory Pointers
//

template<class T>
class VcClassPtr
{
public:
    VcClassPtr()
        : ptr(0) {}
    VcClassPtr(T* rhs)
        : ptr(rhs) {}
    ~VcClassPtr()
    {
        delete ptr;
    }
    VcClassPtr (const VcClassPtr<T>&);
    VcClassPtr (const T& Class)
        : ptr(Class.clone()) {}
    VcClassPtr<T>& operator= (T*);
    VcClassPtr<T>& operator= (const VcClassPtr<T>&);
    T& operator*() const
    {
        return *ptr;
    }
    bool isNull() const
    {
        return ptr == 0;
    }
    T* release ();
    T* operator->() const
    {
        return ptr;
    }
    bool operator== (const VcClassPtr<T>& rhs ) const
    {
        return ptr == rhs.ptr;
    }
    bool operator!= (const VcClassPtr<T>& rhs ) const
    {
        return ptr != rhs.ptr;
    }
protected:
    typename T* ptr;
};
#endif /*#if 0*/

//
// Inlines.
//

template <class T>
inline
CpPtr<T>::CpPtr ()
    : ptr(0)
{}

template <class T>
inline
CpPtr<T>::CpPtr(T* rhs)
    : ptr(rhs)
{}

template <class T>
inline
CpPtr<T>::~CpPtr()
{
    delete ptr;
}

template <class T>
inline
bool
CpPtr<T>::isNull () const
{
    return ptr == 0;
}

template <class T>
inline
CpPtr<T>::CpPtr (const CpPtr<T>& rhs)
{
    ptr = rhs.isNull() ?  0 : new T(*rhs.ptr);

    if (!rhs.isNull() && ptr == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
}

template <class T>
inline
CpPtr<T>&
CpPtr<T>::operator= (const CpPtr<T>& rhs)
{
    if (!(ptr == rhs.ptr))
    {
        delete ptr;
        ptr = rhs.isNull() ? 0 : new T(*rhs.ptr);

        if (!rhs.isNull() && ptr == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
    }
    return *this;
}

template <class T>
inline
CpPtr<T>&
CpPtr<T>::operator= (T* rhs)
{
    delete ptr;
    ptr = rhs;
    return *this;
}

template <class T>
inline
T&
CpPtr<T>::operator* () const
{
    boxAssert(ptr != 0);
    return *ptr;
}

template <class T>
inline
T*
CpPtr<T>::release ()
{
    T* old = ptr;
    ptr = 0;
    return old;
}

template <class T>
inline
bool
CpPtr<T>::operator== (const CpPtr<T>& rhs) const
{
    return ptr == rhs.ptr;
}

template <class T>
inline
bool
CpPtr<T>::operator!= (const CpPtr<T>& rhs) const
{
    return ptr != rhs.ptr;
}

template <class T>
inline
CpClassPtr<T>::CpClassPtr ()
    : CpPtr<T>()
{}

template <class T>
inline
CpClassPtr<T>::CpClassPtr (T* rhs)
    : CpPtr<T>(rhs)
{}

template <class T>
inline
CpClassPtr<T>::CpClassPtr (const CpClassPtr<T>& rhs)
    : CpPtr<T>(rhs) {}

template <class T>
inline
CpClassPtr<T>&
CpClassPtr<T>::operator= (T* rhs)
{
    CpPtr<T>::operator= (rhs);
    return *this;
}

template <class T>
inline
CpClassPtr<T>&
CpClassPtr<T>::operator= (const CpClassPtr<T>& rhs)
{
    CpPtr<T>::operator= (rhs);
    return *this;
}

template <class T>
inline
T*
CpClassPtr<T>::operator-> () const
{
    return this->ptr;
}

template <class T>
inline
LnPtr<T>::LnPtr ()
    : ptr(0)
{}

template <class T>
inline
LnPtr<T>::LnPtr(T* rhs)
    : ptr(rhs)
{}

template <class T>
inline
bool
LnPtr<T>::unique () const
{ 
    return ucnt.unique();
}

template <class T>
inline
LnPtr<T>::~LnPtr ()
{ 
    if (ucnt.unique())
        delete ptr;
}

template <class T>
inline
LnPtr<T>&
LnPtr<T>::operator= (const LnPtr<T>& rhs)
{
    if (ptr != rhs.ptr)
    {
        if (unique())
            delete ptr;
        ptr = rhs.ptr;
        ucnt = rhs.ucnt;
    }
    return *this;
}

template <class T>
inline
int
LnPtr<T>::linkCount () const
{ 
    return ucnt.linkCount();
}

template <class T>
inline
T&
LnPtr<T>::operator* () const
{
    boxAssert(ptr != 0);
    return *ptr;
}

template <class T>
inline
bool
LnPtr<T>::isNull () const
{
    return ptr == 0;
}

template <class T>
inline
bool
LnPtr<T>::operator== (const LnPtr<T>& rhs) const
{
    return ptr == rhs.ptr;
}

template <class T>
inline
bool
LnPtr<T>::operator!= (const LnPtr<T>& rhs) const
{
    return ptr != rhs.ptr;
}

template <class T>
inline
LnClassPtr<T>::LnClassPtr ()
{}

template <class T>
inline
LnClassPtr<T>::LnClassPtr (T* rhs)
    : LnPtr<T>(rhs)
{}

template <class T>
inline
LnClassPtr<T>&
LnClassPtr<T>::operator= (const LnClassPtr<T>& rhs)
{
    LnPtr<T>::operator=(rhs);
    return *this;
}

template <class T>
inline
LnClassPtr<T>&
LnClassPtr<T>::operator= (T* rhs)
{
    LnPtr<T>::operator=(rhs);
    return *this;
}

template <class T>
inline
T*
LnClassPtr<T>::operator->() const
{
    return this->ptr;
}

#ifdef BL_AUTO_INSTANTIATE
#include <Pointers.C>
#endif

#endif /*_POINTERS_H_*/
