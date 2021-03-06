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

#ifndef _MULTIFAB_H_
#define _MULTIFAB_H_

//
// $Id: MultiFab.H,v 1.4 2008/12/15 18:50:38 henshaw Exp $
//

#include <BL_Assert.H>

#include <FArrayBox.H>
#include <FabArray.H>
//
//@Man:
//@Memo: A Collection of FArrayBoxes
/*@Doc:

  The MultiFab class is publically derived from the
  FabArray<Real,FArrayBox> class.  It is a collection (stored as an array) of
  FArrayBoxes useful for storing floating point data on a domain defined by
  a union of rectangular regions embedded in a uniform index space.  The
  MultiFab class extends the function of the underlying FabArray class just
  as the FArrayBox class extends the funtion of BaseFab<Real>.  Additional
  member functions are defined for I/O and simple arithmetic operations on
  these aggregate objects.

  This class does NOT provide a copy constructor or assignment operator.
*/

class MultiFab
    : public FabArray<Real, FArrayBox>
{
  public:

    /*@ManDoc: Constructs an empty MultiFab.  Data can be defined at a later
               time using the `define' member functions inherited
               from FabArray.
    */
    MultiFab ();

    /*@ManDoc: Constructs a MultiFab with a valid region defined by bxs and
               a region of definition defined by the grow factor ngrow.
               If mem\_mode is defined to be Fab\_allocate then FArrayBoxes are
               allocated for each Box in the BoxArray.  The size of the
               FArrayBox is given by the Box grown by ngrow and the number of
               components is given by ncomp.  If mem\_mode is defined to be
               Fab\_noallocate then no FArrayBoxes are allocated at this time
               but can be defined later.  No BoxAssoc is defined but can be
               defined later.
    */
    MultiFab (const BoxArray& bs,
              int             ncomp,
              int             ngrow,
              FabAlloc        mem_mode = Fab_allocate);

    /*@ManDoc: This constructor is similar to the previous one except that it
               also specifies a BoxAssoc.  The valid region is defined by the
               BoxArray contained in the BoxAssoc.
    */
    MultiFab (const BoxAssoc& ba,
              int             ncomp,
              int             ngrow,
              FabAlloc        mem_mode = Fab_allocate);

    /*@ManDoc: Constructs a MultiFab by reading it from the input stream strm.
               This is useful when restarting a calculation from a checkpoint
               file.  See the `writeOn' and `readFrom' member functions below.
    */
    MultiFab (istream& strm);
    //
    //@ManDoc: The (virtual) destructor.
    //
    virtual ~MultiFab();

    /*@ManDoc: This function writes the state of the MultiFab to the given
               output stream strm.  The MultiFab can be read in from a disk
               file at a later time using either the `readFrom' function or the
               last constructor defined above.  It writes informational data
               as a header in ASCII then calls the FArrayBox::writeOn function
               for each element of the MultiFab.  It returns a reference to
               the ostream strm to allow for operation chaining.
    */
    std::ostream& writeOn (std::ostream& strm) const;

    /*@ManDoc: This is just the opposite of `writeOn' defined above.  It
               assumes the MultiFab is empty and constructs the BoxArray and
               FArrayBoxes by reading them from the input stream strm.  It
               returns a reference to the istream strm to allow for
               operation chaining.
    */
    istream& readFrom (istream& strm);

    /*@ManDoc: This writes out the entire contents of a MultiFab, one line for
               each cell of data, in an ASCII format.  It is included mainly
               for completeness.  It is not recommended for general use.
    */
    friend std::ostream& operator<< (std::ostream&        strm,
                                const MultiFab& mf);

    /*@ManDoc: This is a diagnostic function.  It writes (in ASCII) the state
               of the cell in the MultiFab with index location given by pnt
               to the output stream strm.
    */
    void probe (std::ostream& strm,
                IntVect& pt);

    /*@ManDoc: Returns the minimum value contained in component comp of the
               MultiFab.  The parameter nghost determines the number of
               boundary cells to search for the minimum.  The default is to
               search only the valid regions of the FArrayBoxes.
    */
    Real min (int comp,
              int nghost = 0) const;

    /*@ManDoc: Identical to the previous min() function, but confines its
               search to intersection of Box b and the MultiFab.
    */
    Real min (const Box& b,
              int        comp,
              int        nghost = 0) const;

    /*@ManDoc: Returns the maximum value contained in component comp of the
               MultiFab.  The parameter nghost determines the number of
               boundary cells to search for the maximum.  The default is to
               search only the valid regions of the FArrayBoxes.
    */
    Real max (int comp,
              int nghost = 0) const;

    /*@ManDoc: Identical to the previous max() function, but confines its
               search to intersection of Box b and the MultiFab.
    */
    Real max (const Box& b,
              int        comp,
              int        nghost = 0) const;

    /*@ManDoc: Adds the scalar value val to the value of each cell in the
               specified subregion of the MultiFab.  The subregion consists
               of the num\_comp components starting at component comp.
               The value of nghost specifies the number of cells in the
               boundary region of each FArrayBox in the subregion that should
               be modified.  
    */
    void plus (Real val,
               int  comp,
               int  num_comp,
               int  nghost = 0);

    /*@ManDoc: Identical to the previous version of plus(), with the
               restriction that the subregion is further constrained to
               the intersection with Box region.
    */
    void plus (Real       val,
               const Box& region,
               int        comp,
               int        num_comp,
               int        nghost = 0);

    /*@ManDoc: Adds the scalar value val to the value of each cell in the
               valid region of each component of the MultiFab.  The value
               of nghost specifies the number of cells in the boundary
               region that should be modified.
    */
    void plus (Real val,
               int  nghost);

    /*@ManDoc: Adds the scalar value val to the value of each cell in the
               valid region of each component of the MultiFab, that also
               intersects the Box region.  The value of nghost specifies the
               number of cells in the boundary region of each FArrayBox in
               the subregion that should be modified.

    */
    void plus (Real       val,
               const Box& region,
               int        nghost);

    /*@ManDoc: Scales the value of each cell in the specified subregion of the
               MultiFab by the scalar val (a[i] <- a[i]*val). The subregion
               consists of the num\_comp components starting at component comp.
               The value of nghost specifies the number of cells in the
               boundary region of each FArrayBox in the subregion that should
               be modified.  
    */
    void mult (Real val,
               int  comp,
               int  num_comp,
               int  nghost = 0);

    /*@ManDoc: Identical to the previous version of mult(), with the
               restriction that the subregion is further constrained to the
               intersection with Box region.  The value of nghost specifies the
               number of cells in the boundary region of each FArrayBox in
               the subregion that should be modified.
    */
    void mult (Real       val,
               const Box& region,
               int        comp,
               int        num_comp,
               int        nghost = 0);

    /*@ManDoc: Scales the value of each cell in the valid region of each
               component of the MultiFab by the scalar val (a[i] <- a[i]*val).
               The value of nghost specifies the number of cells in the
               boundary region that should be modified.
    */
    void mult (Real val,
               int  nghost);

    /*@ManDoc: Scales the value of each cell in the valid region of each
               component of the MultiFab by the scalar val (a[i] <- a[i]*val),
               that also intersects the Box region.  The value of nghost
               specifies the number of cells in the boundary region of each
               FArrayBox in the subregion that should be modified.
    */
    void mult (Real       val,
               const Box& region,
               int        nghost);

    /*@ManDoc: Replaces the value of each cell in the specified subregion of
               the MultiFab with its reciprocal multiplied by the value of
               numerator. The subregion consists of the num\_comp components
               starting at component comp.  The value of nghost specifies the
               number of cells in the boundary region of each FArrayBox in the
               subregion that should be modified.
    */
    void invert (Real numerator,
                 int  comp,
                 int  num_comp,
                 int  nghost = 0);

    /*@ManDoc: Identical to the previous version of invert(), with the
               restriction that the subregion is further constrained to the
               intersection with Box region.  The value of nghost specifies the
               number of cells in the boundary region of each FArrayBox in the
               subregion that should be modified.
    */
    void invert (Real       numerator,
                 const Box& region,
                 int        comp,
                 int        num_comp,
                 int        nghost = 0);

    /*@ManDoc: Replaces the value of each cell in the specified subregion of
               the MultiFab with its reciprocal multiplied by the value of
               numerator.  The value of nghost specifies the number of cells
               in the boundary region that should be modified.
    */
    void invert (Real numerator,
                 int  nghost);

    /*@ManDoc: Replaces the value of each cell in the specified subregion of
               the MultiFab, that also intersects the Box region, with its
               reciprocal multiplied by the value of numerator.  The value
               of nghost specifies the number of cells in the boundary region
               of each FArrayBox in the subregion that should be modified.
    */
    void invert (Real       numerator,
                 const Box& region,
                 int        nghost);

    /*@ManDoc: Negates the value of each cell in the specified subregion of
               the MultiFab.  The subregion consists of the num\_comp
               components starting at component comp.  The value of nghost
               specifies the number of cells in the boundary region of each
               FArrayBox in the subregion that should be modified.  
    */
    void negate (int comp,
                 int num_comp,
                 int nghost = 0);

    /*@ManDoc: Identical to the previous version of negate(), with the
               restriction that the subregion is further constrained to
               the intersection with Box region.
    */
    void negate (const Box& region,
                 int        comp,
                 int        num_comp,
                 int        nghost = 0);

    /*@ManDoc: Negates the value of each cell in the valid region of
               the MultiFab.  The value of nghost specifies the number of
               cells in the boundary region that should be modified.  
    */
    void negate (int nghost = 0);

    /*@ManDoc: Negates the value of each cell in the valid region of
               the MultiFab that also intersects the Box region.  The value
               of nghost specifies the number of cells in the boundary region
               that should be modified.  
    */
    void negate (const Box& region,
                 int        nghost = 0);

    /* This function adds the values of the cells in mf to the corresponding
       cells of this MultiFab.  mf is required to have the same BoxArray or
       "valid region" as this MultiFab.  The addition is done only to num\_comp
       components, starting with component number strt\_comp.  The parameter
       nghost specifies the number of boundary cells that will be modified.
       If nghost == 0, only the valid region of each FArrayBox will be
       modified.
    */
    void plus (const MultiFab& mf,
               int             strt_comp,
               int             num_comp,
               int             nghost);

    /* This function subtracts the values of the cells in mf from the
       corresponding cells of this MultiFab.  mf is required to have the
       same BoxArray or "valid region" as this MultiFab.  The subtraction is
       done only to num\_comp components, starting with component number
       strt\_comp.  The parameter nghost specifies the number of boundary
       cells that will be modified.  If nghost == 0, only the valid region of
       each FArrayBox will be modified.
    */
    void minus (const MultiFab& mf,
                int             strt_comp,
                int             num_comp,
                int             nghost);

    /*@ManDoc: This function performs a linear interpolation of values between
               corresponding cells of the MultiFabs f1 and f2.  The
               interpolation is done on the intersection of the two MultiFabs
               with the Box given by subbox (which is contained in dest).
               The results are stored in the FArrayBox dest.  The two
               MultiFabs are required to have the same underlying BoxArray
               (or valid region).  The parameters t, t1, and t2 are expected
               to have the relation t1 <= t <= t2.  The interpolated value of
               a cell is given by: f1*(t2-t)/(t2-t1)+f2*(t-t1)/(t2-t1).
               If t<t1 or t>t2 and extrap == true, then linear extrapolation
               using the same formula is used.
    */
    friend void linInterp (FArrayBox&      dest,
                           const Box&      subbox,
                           const MultiFab& f1,
                           const MultiFab& f2,
                           Real            t1,
                           Real            t2,
                           Real            t,
                           bool            extrap );

    /*@ManDoc: This function is identical to the previous one except that
               the interpolation is performed on only num\_comp components
               starting with MultiFab component src\_comp.  The resulting
               data is written into dest starting at component dest\_comp.
    */
    friend void linInterp (FArrayBox&      dest,
                           const Box&      subbox,
                           const MultiFab& f1,
                           const MultiFab& f2,
                           Real            t1,
                           Real            t2,
                           Real            t,
                           int             src_comp,
                           int             dest_comp,
                           int             num_comp,
                           bool            extrap );

    /*@ManDoc: This function is identical to the first linInterp() function
               discussed above except that dest has the same valid region as
               the Kth FArrayBox in the MultiFabs.  The BoxAssoc ba defines a
               nearest neighbor list for the Kth FArrayBox.  This is a more
               efficient operation since it narrows the search for which
               FArrayBoxes will participate in the interpolation.
    */
    friend void linInterp (FArrayBox&      dest,
                           const Box&      subbox,
                           const BoxAssoc& ba,
                           int             K,
                           const MultiFab& f1,
                           const MultiFab& f2,
                           Real            t1,
                           Real            t2,
                           Real            t,
                           bool            extrap );

    /*@ManDoc: This is a version of linInterp() that combines all the
               features of the previous two.
    */
    friend void linInterp (FArrayBox&      dest,
                           const Box&      subbox,
                           const BoxAssoc& ba,
                           int             Box_assoc_index,
                           const MultiFab& f1,
                           const MultiFab& f2,
                           Real            t1,
                           Real            t2,
                           Real            t,
                           int             src_comp,
                           int             dest_comp,
                           int             num_comp,
                           bool            extrap );

private:
    //
    // These are disabled.
    //
    MultiFab (const MultiFab&);
    MultiFab& operator= (const MultiFab&);
};

//
// Inlines.
//

inline
void
MultiFab::plus (Real val,
                int  nghost)
{
    plus(val,0,n_comp,nghost);
}

inline
void
MultiFab::plus (Real       val,
                const Box& region,
                int        nghost)
{
    plus(val,region,0,n_comp,nghost);
}

inline
void
MultiFab::mult (Real val,
                int  nghost)
{
    mult(val,0,n_comp,nghost);
}

inline
void
MultiFab::mult (Real       val,
                const Box& region,
                int        nghost)
{
    mult(val,region,0,n_comp,nghost);
}

inline
void
MultiFab::invert (Real numerator,
                  int  nghost)
{
    invert(numerator,0,n_comp,nghost);
}

inline
void
MultiFab::invert (Real       numerator,
                  const Box& region,
                  int        nghost)
{
    invert(numerator,region,0,n_comp,nghost);
}

inline
void
MultiFab::negate (int nghost)
{
    negate(0,n_comp,nghost);
}

inline
void
MultiFab::negate (const Box& region,
                  int        nghost)
{
    negate(region,0,n_comp,nghost);
}

#endif /*_MULTIFAB_H_*/
