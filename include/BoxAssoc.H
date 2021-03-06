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

#ifndef _BOXASSOC_H_
#define _BOXASSOC_H_

//
// $Id: BoxAssoc.H,v 1.4 2008/12/03 17:54:46 chand Exp $
//

#include <Boolean.H>
#include <BL_Assert.H>
#include <BoxLib.H>
#include <BoxList.H>
#include <BoxArray.H>
#include <Array.H>

#if 0
  -----------------------------------------------------------------------
  This class implements "nearest neighbor" lists for boxes in a BoxArray,
  or between two boxarrays.  In fact, each object of this class may contain
  multiple nearest neighbor lists, one for each defined cachewidth.

  Box Y is a neighbor of Box X with cachewidth = N if

     Y.intersects(grow(X,N)) is true.

 One can define as many cachewidths (and the associated neighbor lists)
 as necessary.  These lists are stored and available for inspection
 using the nborMax and nborIndex functions:

    nborMax(k,cw) -- returns the number of neighbors of the kth
                     box in the list with cachewidth = cw.

    nborIndex(k,i,cw) -- returns the index of the ith neighbor of the
                         kth box in the list with cachewidth=cw.

 For simplicity, one can define a default list by a call to
 setCacheWidth.  Version of nborMax and nborIndex access the
 default neighbor list:

    setCacheWidth(cw) -- sets list with cachewidth=cw as default.
    nborMax(k) -- returns number of neighbord os kth box in default list.
    nborIndex(k,i) -- return the ith neighbor of the kth box in the
                      default list.

 NOTE:  this is the preferred usage since it is a bit more efficient.
 -----------------------------------------------------------------------

 Example usage:

 void foo (const BoxArray& bs)
 {
     //
     // Build box association with cache width of 1.
     // This is the default neighbor list.
     //
     BoxAssoc ba(bs,1);
     //
     // Build another association list with cachewidth = 3
     // (this is now the default list).
     //
     ba.setCacheWidth(3);
     //
     // Reset default list to that with cachewidth of 1.
     // Note that since the list already exists it is not built.
     //
     ba.setCacheWidth(1);
     //
     // Walk neighbor list.
     //
     for (int k = 0; k < ba.length(); k++)
     {
         //
         // Get reference to k'th box.
         //
         const Box& box_k = ba[k];
         //
         // Get number of neighbors of box_k.
         //
         int num_k = ba.nborMax(k);
         //
         // Get neighbors of box_k.
         //
         for (int i = 0; i < num_k; i++)
         {
             //
             // Get index of ith neighbor.
             //
             int nbor = ba.nborIndex(k,i);
             //
             // Get reference to ith neighbor.
             //
             const Box& box_i = ba[nbor];
             //
             // Do stuff ...
             //
         }
     }
 }
#endif

//
//@Man:
//@Memo: Nearest Neighbor Lists for Boxes and BoxArrays
/*@Doc:

  This class implements "nearest neighbor" lists for Boxes in a BoxArray, or
  between two BoxArrays.  Each object of this class may contain multiple
  nearest neighbor lists, one for each specified cachewidth.

  Box Y is a neighbor of Box X with cachewidth == N if

    Y.intersects(grow(X,N))

  is true.

  One can define as many cachewidths (and the associated neighbor lists) as
  necessary.  These lists are stored and available for inspection using the
  nborMax() and nborIndex() functions.
*/

class BoxAssoc
{
public:
    //
    // Used internally by BoxAssoc.
    //
    class indexVect
    {
        friend class BoxAssoc;
        friend std::ostream& operator << (std::ostream&,
                                     const BoxAssoc& );
    private:
        indexVect ()
            : nelem(0), vec(NULL)
        {}
        indexVect (int len);
        indexVect (const indexVect& iv,
                   int              len);
        ~indexVect ()
        {
            delete [] vec;
        }
        void copy (const indexVect& iv,
                   int              len);
        void resize (int newlen);
        int size () const
        {
            return nelem;
        }
        int operator [] (int index) const
        {
            return vec[index];
        }
    private:
        int  nelem;
        int* vec;
    };
    //
    // Used internally by BoxAssoc.
    //
    struct BARec
    {
        int        cwidth;
        indexVect* lst;
        BARec*     next;
    };
public:
    //
    //@ManDoc: Build an empty BoxAssoc.
    //
    BoxAssoc ();
    //
    //@ManDoc: The copy constructor.
    //
    BoxAssoc (const BoxAssoc& rhs);
    //
    //@ManDoc: Construct a BoxAssoc from a BoxArray for given cachewidth.
    //
    BoxAssoc (const BoxArray& bxs,
              int             cachewidth = 0);
    //
    //@ManDoc: Construct a BoxAssoc from two BoxArrays for given cacehwidth.
    //
    BoxAssoc (const BoxArray& bxs1,
              const BoxArray& bxs2,
              int             cachewidth = 0);
    //
    //@ManDoc: The assignment operator.
    //
    BoxAssoc& operator= (const BoxAssoc& rhs);
    //
    //@ManDoc: The destructor.
    //
    ~BoxAssoc ();
    //
    //@ManDoc: Returns true if this BoxAssoc is valid.
    //
    bool ready () const;
    //
    //@ManDoc: Delete all lists and associations with any BoxArray(s).
    //
    void clear ();

    /*@ManDoc: Deletes BoxArray and all lists then rebuilds itself as an
               exact copy of argument.
    */
    void copy (const BoxAssoc& ba);
    //
    //@ManDoc: Initialize this BoxAssoc after default construction.
    //
    void define (const BoxArray& bxs,
                 int             cachewidth = 0);
    //
    //@ManDoc: Initialize this BoxAssoc after default construction.
    //
    void define (const BoxArray& bxs1,
                 const BoxArray& bxs2,
                 int             cachewidth = 0);

    /*@ManDoc: Set the  default cachewidth.  If it not already constructed
               this will construct the association list and add it to the
               known lists.
    */
    void setCacheWidth (int cachewidth);
    //
    //@ManDoc: Return the current cachewidth.
    //
    int cacheWidth () const;

    /*@ManDoc: Return number of neighbors of box indicated by index for
               the current cachewidth.
    */
    int nborMax (int index) const;

    /*@ManDoc: Return number of neighbors of box indicated by index for
               the given cachewidth.
    */
    int nborMax (int index,
                 int cachewidth) const;

    /*@ManDoc: Return index of count'th neighbor of box at index boxindex
               for the current cachewidth.
    */
    int nborIndex (int boxindex,
                   int count) const;

    /*@ManDoc: Return index of count'th neighbor of box at index boxindex
               for the given cachewidth.
    */
    int nborIndex (int boxindex,
                   int count,
                   int cachewidth) const;
    //
    //@ManDoc: Am I an association between two different BoxArrays?
    //
    bool isSelfAssociaton () const;
    //
    //@ManDoc: The length of the associated BoxArray.
    //
    int length () const;
    //
    //@ManDoc: The length of the other associated BoxArray (if it exists).
    //
    int otherLength () const;
    //
    //@ManDoc: Returns the Box at the requested index.
    //
    const Box& operator[] (int index) const;
    //
    //@ManDoc: Returns the Box at the requested index.
    //
    const Box& get (int index) const;
    //
    //@ManDoc: Returns the Box at the requested index for the "other" BoxArray.
    //
    const Box& getOther (int index) const;
    //
    //@ManDoc: Returns the BoxArray.
    //
    const BoxArray& boxArray () const;
    //
    //@ManDoc: Returns the "other" BoxArray.
    //
    const BoxArray& otherBoxArray () const;
    //
    //@ManDoc: Switch "main" and "other" BoxArrays.
    //
    BoxAssoc transpose () const;
    //
    //@ManDoc: Write a BoxAssoc to an ostream in ASCII format.
    //
    friend std::ostream& operator<< (std::ostream&        os,
                                const BoxAssoc& ba);
private:
    //
    // These functions are for internal use only.
    //
    BARec* getCW (int cachewidth) const;
    void makeneighbors (indexVect* lst,
                        int        cwidth);
    BoxArray* bs;
    BARec*    current;
    BARec*    first;
    BoxArray* bso;
};

//
// Inlines.
//

inline
int
BoxAssoc::length () const
{
    boxAssert(bs != 0);
    return bs->length();
}

inline
int
BoxAssoc::otherLength () const
{
    boxAssert(bso != 0);
    return bso->length();
}

inline
const Box&
BoxAssoc::operator[] (int index) const
{
    boxAssert(bs != 0);
    return bs->get(index);
}

inline
const Box&
BoxAssoc::get (int index) const
{
    boxAssert(bs != 0);
    return bs->get(index);
}

inline
const Box&
BoxAssoc::getOther (int index) const
{
    boxAssert(bso != 0);
    return bso->get(index);
}

inline
const BoxArray&
BoxAssoc::boxArray () const
{
    boxAssert(bs != 0);
    return *bs;
}

inline
const BoxArray&
BoxAssoc::otherBoxArray () const
{
    boxAssert(bso != 0);
    return *bso;
}

inline
void
BoxAssoc::define (const BoxArray& bxs,
                  int             cw)
{
    boxAssert(bs == 0 || (bs == &bxs));
    if (bs == 0)
    {
        bs = new BoxArray(bxs);
        if (bs == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        bso = bs;
    }
    setCacheWidth(cw);
}

inline
void
BoxAssoc::define (const BoxArray& bxs,
                  const BoxArray& bxso,
                  int             cw)
{
    boxAssert(bs == 0  || bs == &bxs);
    boxAssert(bso == 0 || bso == &bxso);
    if (bs == 0)
        bs = new BoxArray(bxs);
    if (bso == 0)
        bso = new BoxArray(bxso);
    if (bs == 0 || bso == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    setCacheWidth(cw);
}

inline
int
BoxAssoc::nborIndex (int boxindex,
                     int count) const
{
    boxAssert(current != 0);
    boxAssert(boxindex >= 0 && boxindex < length());
#ifndef NDEBUG
    const indexVect& p = current->lst[boxindex];
    boxAssert(count >= 0 && count < p.size());
    return p[count];
#else
    return current->lst[boxindex][count];
#endif
}

inline
int
BoxAssoc::nborIndex (int boxindex,
                     int count,
                     int cw) const
{
    boxAssert(boxindex >= 0 && boxindex < length() );
    BARec* r = getCW(cw);
    boxAssert(r != 0);
#ifndef NDEBUG
    const indexVect& p = r->lst[boxindex];
    boxAssert(count >= 0 && count < p.size() );
    return p[count];
#else
    return r->lst[boxindex][count];
#endif
}

inline
int
BoxAssoc::cacheWidth () const
{
    boxAssert(current != 0);
    return current->cwidth;
}

inline
int
BoxAssoc::nborMax (int index) const
{
    boxAssert(current != 0);
    return current->lst[index].size();
}

inline
int
BoxAssoc::nborMax (int index,
                   int cachewidth) const
{
    BARec *r = getCW(cachewidth);
    boxAssert(r != 0);
    return r->lst[index].size();
}

inline
BoxAssoc
BoxAssoc::transpose () const
{
    boxAssert(bso->ready());
    boxAssert(bs->ready());
    return BoxAssoc(otherBoxArray(), boxArray(), cacheWidth());
}

inline
bool
BoxAssoc::isSelfAssociaton () const
{
    return bs == bso;
}

#endif /*_BOXASSOC_H_*/
