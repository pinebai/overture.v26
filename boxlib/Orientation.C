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

//
// $Id: Orientation.C,v 1.2 2008/12/03 17:54:46 chand Exp $
//

#include <BoxLib.H>
#include <Orientation.H>
#include <Utility.H>

using namespace std;

ostream&
operator<< (ostream&           os,
            const Orientation& o)
{
    os << '('<< o.val << ')' ;
    if (os.fail())
        BoxLib::Error("operator<<(ostream&,Orientation&) failed");
    return os;
}

istream&
operator>> (istream&     is,
            Orientation& o)
{
    char c;
    is >> c;
    is.putback(c);
    if (c == '(')
    {
        is.ignore(BL_IGNORE_MAX, '(');
        is >> o.val;
        is.ignore(BL_IGNORE_MAX, ')');
    }
    else
        BoxLib::Error("operator>>(istream&,Orientation&): expected \'(\'");

    if (is.fail())
        BoxLib::Error("operator>>(ostream&,Orientation&) failed");

    return is;
}
