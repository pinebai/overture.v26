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

#ifndef FPC_H
#define FPC_H

//
// $Id: FPC.H,v 1.1.1.1 1999/11/17 19:47:23 henshaw Exp $
//

#include <FabConv.H>

//
//@Man:
//@Memo: A Collection of Floating-Point Constants Supporting FAB I/O
/*@Doc:

  This class is a poor-person's namespace of floating-point constants
  used in support of FAB I/O.  Since we can't assume the existence of
  namespaces, and we don't like global constants, we make them static
  constant data members of this class.
*/

class FPC
{
public:
    //
    //@ManDoc: 4-element array representing "normal" float order: {1,2,3,4}
    //
    static const int normal_float_order[];
    //
    //@ManDoc: 4-element array representing "reverse" float order: {4,3,2,1}
    //
    static const int reverse_float_order[];
    //
    //@ManDoc: Another "reversed" FP order: {2,1,4,3}
    //
    static const int reverse_float_order_2[];
    //
    //@ManDoc: The "normal" double order: {1,2,3,4,5,6,7,8}
    //
    static const int normal_double_order[];
    //
    //@ManDoc: A "reversed" double order: {8,7,6,5,4,3,2,1}
    //
    static const int reverse_double_order[];
    //
    //@ManDoc: Another "reversed" double order: {2,1,4,3,6,5,8,7}
    //
    static const int reverse_double_order_2[];
    //
    //@ManDoc: The Cray float order: {1,2,3,4,5,6,7,8}
    //
    static const int cray_float_order[];

    /*@ManDoc: Array detailing the format of IEEE 32-bit normal order floats.

       In general, here's what the various indices in "format" array means:

          format[0] = number of bits per number
          format[1] = number of bits in exponent
          format[2] = number of bits in mantissa
          format[3] = start bit of sign
          format[4] = start bit of exponent
          format[5] = start bit of mantissa
          format[6] = high order mantissa bit (CRAY needs this)
          format[7] = bias of exponent
    */
    static const long ieee_float[];
    //
    //@ManDoc: Array detailing the format of IEEE 64-bit normal order doubles.
    //
    static const long ieee_double[];
    //
    //@ManDoc: Array detailing the format of Cray floating point.
    //
    static const long cray_float[];

    /*@ManDoc: Returns a constant reference to an IntDescriptor describing
               the native "long" under which BoxLib was compiled.  Each
               BoxLib library will have exactly one of these compiled into it.
    */
    static const IntDescriptor&  NativeLongDescriptor ();

    /*@ManDoc: Returns a constant reference to a RealDescriptor describing
               the native Real under which BoxLib was compiled.  Each BoxLib
               library will have exactly one of these compiled into it.
               Note that "Real" will be one of "float" or "double" depending
               on how the version of BoxLib was built.
    */
    static const RealDescriptor& NativeRealDescriptor ();

    /*@ManDoc: Returns a constant reference to a RealDescriptor detailing
               the Cray FP format.
    */
    static const RealDescriptor& CrayRealDescriptor ();

    /*@ManDoc: Returns a constant reference to a RealDescriptor detailing
               the IEEE 32-bit normal FP format.
    */
    static const RealDescriptor& Ieee32NormalRealDescriptor ();

    /*@ManDoc: Returns a constant reference to a RealDescriptor detailing
               the IEEE 64-bit normal FP format.
    */
    static const RealDescriptor& Ieee64NormalRealDescriptor ();
};

#endif /*FPC_H*/
