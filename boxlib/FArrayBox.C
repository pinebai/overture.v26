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
// $Id: FArrayBox.C,v 1.4 2008/12/03 17:54:46 chand Exp $
//

#include <stdlib.h>
//kkc 081124 #include <iostream.h>
#include <iostream>
//kkc 081124 #include <iomanip.h>
#include <iomanip>
using namespace std;


#if defined(BL_Solaris) && defined(__PHOTON)
#undef __P
#endif

#include <math.h>
#include <string.h>

#include <Misc.H>
#include <FArrayBox.H>
#include <FabConv.H>
#include <ParmParse.H>
#include <FabConv.H>
#include <FPC.H>

#include <BL_Assert.H>
#include <BoxLib.H>
#include <Looping.H>
#include <Utility.H>

#if defined(BL_ARCH_CRAY)
#ifdef BL_USE_DOUBLE
#error DOUBLE not allowed on CRAY
#endif
#endif

//
// This stuff exists to support reading of old FABs.
//

#if defined(BL_ARCH_IEEE)
   static const char sys_name[] = "IEEE";
#elif defined(BL_ARCH_CRAY)
   static const char sys_name[] = "CRAY";
#endif

//
// Default format to Native FP type.
//
FABio::Format FArrayBox::format = FABio::FAB_NATIVE;

//
// Default Ordering to Normal Order.
//
FABio::Ordering FArrayBox::ordering = FABio::FAB_NORMAL_ORDER;

//
// Our 8-bit FABio type.
//
class FABio_8bit
    : public FABio
{
public:
    virtual void read (istream&   is,
                       FArrayBox& fb) const;
    virtual void write (ostream&         os,
                        const FArrayBox& fb,
                        int              comp,
                        int              num_comp) const;
    virtual void skip (istream&   is,
                       FArrayBox& f) const;
protected:
    virtual void write_header (ostream&         os,
                               const FArrayBox& f,
                               int              nvar) const;
};

//
// Our ASCII FABio type.
//
class FABio_ascii
    : public FABio
{
public:
    virtual void read (istream&   is,
                       FArrayBox& fb) const;
    virtual void write (ostream&         os,
                        const FArrayBox& fb,
                        int              comp,
                        int              num_comp) const;
    virtual void skip (istream&   is,
                       FArrayBox& f) const;
protected:
    virtual void write_header (ostream&         os,
                               const FArrayBox& f,
                               int              nvar) const;
};

//
// Our binary FABio type.
//
class FABio_binary
    : public FABio
{
public:
    FABio_binary (RealDescriptor* rd_);

    virtual void read (istream&   is,
                       FArrayBox& fb) const;
    virtual void write (ostream&         os,
                        const FArrayBox& fb,
                        int              comp,
                        int              num_comp) const;
    virtual void skip (istream&   is,
                       FArrayBox& f) const;

protected:
    virtual void write_header (ostream&         os,
                               const FArrayBox& f,
                               int              nvar) const;

    CpClassPtr<RealDescriptor> rd;
};

//
// This isn't inlined as it's virtual.
//

FABio::~FABio ()
{}

void
FABio::write_header (ostream&         os,
                     const FArrayBox& f,
                     int              nvar) const
{
    boxAssert(nvar <= f.nComp());
    os << f.box() << ' ' << nvar << '\n';
    if (os.fail())
        BoxLib::Error("FABio::write_header(ostream&,FArrayBox&,int) failed");
}

//
// The default FABio pointer is NATIVE.
//
FABio* FArrayBox::fabio = new FABio_binary(FPC::NativeRealDescriptor().clone());

FArrayBox::FArrayBox ()
    : BaseFab<Real>()
{}

FArrayBox::FArrayBox (const Box& b,
                      int        n)
    : BaseFab<Real>(b,n)
{}

void
FArrayBox::setFormat (FABio::Format fmt)
{
    FABio* fio = 0;

    switch (fmt)
    {
    case FABio::FAB_ASCII: fio = new FABio_ascii; break;
    case FABio::FAB_8BIT:  fio = new FABio_8bit;  break;
    case FABio::FAB_NATIVE:
        fio = new FABio_binary(FPC::NativeRealDescriptor().clone());
        break;
    case FABio::FAB_IEEE:
        BoxLib::Warning("FABio::FAB_IEEE has been deprecated");
        //
        // Fall through ...
        //
    case FABio::FAB_IEEE_32:
        fio = new FABio_binary(FPC::Ieee32NormalRealDescriptor().clone());
        break;
    default:
        cerr << "FArrayBox::setFormat(): Bad FABio::Format = " << fmt;
        BoxLib::Abort();
    }

    if (fio == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);

    setFABio(fio);
}

void
FArrayBox::setOrdering (FABio::Ordering ordering_)
{
    BoxLib::Warning("FArrayBox::setOrdering() has been deprecated");
    ordering = ordering_;
}

FABio::Ordering
FArrayBox::getOrdering ()
{
    BoxLib::Warning("FArrayBox::getOrdering() has been deprecated");
    return ordering;
}

void
FArrayBox::setPrecision (FABio::Precision)
{
    BoxLib::Warning("FArrayBox::setPrecision() has been deprecated");
}

FABio::Precision
FArrayBox::getPrecision ()
{
    BoxLib::Warning("FArrayBox::getPrecision() has been deprecated");
    return FABio::FAB_FLOAT;
}

#ifdef BL_USE_POINTLIB
#ifndef BL_CRAY_BUG_DEFARG
FArrayBox::FArrayBox (const PointFab<PointDomain>& pf,
                      Real                         val)
    : BaseFab<Real>(pf,val)
{}
#endif /*BL_CRAY_BUG_DEFARG*/
#endif /*BL_USE_POINTLIB*/

FArrayBox::FArrayBox (BaseFab<Real>& bf,
                      Box            subb,
                      int            ns,
                      int            nc)
    : BaseFab<Real>(bf,subb,ns,nc)
{}

//
// This isn't inlined as it's virtual.
//

FArrayBox::~FArrayBox ()
{}

void
FArrayBox::init ()
{
    ParmParse pp("fab");

    aString ord, fmt;

    //
    // This block can legitimately set FAB output format.
    //
    if (pp.query("format", fmt))
    {
        FABio* fio = 0;

        if (fmt == "ASCII")
            fio = new FABio_ascii;
        else if (fmt == "8BIT")
            fio = new FABio_8bit;
        else if (fmt == "NATIVE")
            fio = new FABio_binary(FPC::NativeRealDescriptor().clone());
        else if (fmt == "IEEE" || fmt == "IEEE32")
        {
            if (fmt == "IEEE")
                BoxLib::Warning("IEEE fmt in ParmParse files is deprecated");
            fio = new FABio_binary(FPC::Ieee32NormalRealDescriptor().clone());
        }
        else
        {
            cerr << "FArrayBox::init(): Bad FABio::Format = " << fmt;
            BoxLib::Abort();
        }

        if (fio == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);

        setFABio(fio);
    }

    //
    // This block sets ordering which doesn't affect output format.
    // It is only used when reading in an old FAB.
    //
    if (pp.query("ordering", ord))
    {
        if (ord == "NORMAL_ORDER")
            FArrayBox::setOrdering(FABio::FAB_NORMAL_ORDER);
        else if (ord == "REVERSE_ORDER")
            FArrayBox::setOrdering(FABio::FAB_REVERSE_ORDER);
        else if (ord == "REVERSE_ORDER_2")
            FArrayBox::setOrdering(FABio::FAB_REVERSE_ORDER_2);
        else
        {
            cerr << "FArrayBox::init(): Bad FABio::Ordering = " << ord;
            BoxLib::Abort();
        }
    }
}

Real
FArrayBox::norm (int p,
                 int comp,
                 int numcomp) const
{
    return norm(domain,p,comp,numcomp);
}

Real
FArrayBox::norm (const Box& subbox,
                 int        p,
                 int        comp,
                 int        numcomp) const
{
    boxAssert(comp >= 0 && comp+numcomp <= nComp());
    boxAssert(p >= 0);

    Real* tmp = 0;
    int tmplen = 0;
    Real nrm = 0;
    if (p == 0 || p == 1)
        nrm = NormedFab<Real>::norm(subbox, p, comp, numcomp);
    else if (p == 2)
    {
        ForAllThisCPencil(Real,subbox,comp,numcomp)
        {
            const Real* row = &thisR;
            if (tmp == 0)
            {
                tmp = new Real[thisLen];
                if (tmp == 0)
                    BoxLib::OutOfMemory(__FILE__, __LINE__);
                tmplen = thisLen;
                for (int i = 0; i < thisLen; i++)
                    tmp[i] = row[i]*row[i];
            }
            else
            {
                for (int i = 0; i < thisLen; i++)
                    tmp[i] += row[i]*row[i];
            }
        } EndForPencil
        nrm = tmp[0];
        for (int i = 1; i < tmplen; i++)
            nrm += tmp[i];
        nrm = sqrt(nrm);
    }
    else
    {
        Real pwr = Real(p);
        ForAllThisCPencil(Real,subbox,comp,numcomp)
        {
            const Real* row = &thisR;
            if (tmp == 0)
            {
                tmp = new Real[thisLen];
                if (tmp == 0)
                    BoxLib::OutOfMemory(__FILE__, __LINE__);
                tmplen = thisLen;
                for (int i = 0; i < thisLen; i++)
                    tmp[i] = pow(row[i],pwr);
            }
            else
            {
                for (int i = 0; i < thisLen; i++)
                    tmp[i] += pow(row[i],pwr);
            }
        } EndForPencil
        nrm = tmp[0];
        for (int i = 1; i < tmplen; i++)
            nrm += tmp[i];
        Real invpwr = 1.0/pwr;
        nrm = pow(nrm,invpwr);
    }
    delete [] tmp;
    return nrm;
}

//
// This is where lies all the smarts for recognizing FAB headers.
//

FABio*
FABio::read_header (istream&   is,
                    FArrayBox& f)
{
    int nvar;
    Box bx;
    FABio* fio = 0;
    char c;

    is >> c;
    if (c != 'F') BoxLib::Error("FABio::read_header(): expected \'F\'");
    is >> c;
    if (c != 'A') BoxLib::Error("FABio::read_header(): expected \'A\'");
    is >> c;
    if (c != 'B') BoxLib::Error("FABio::read_header(): expected \'B\'");

    is >> c;
    if (c == ':')
    {
        //
        // The "old" FAB format.
        //
        int typ_in, wrd_in;
        is >> typ_in;
        is >> wrd_in;

        char machine[128];
        is >> machine;
        is >> bx;
        is >> nvar;
        //
        // Set the FArrayBox to the appropriate size.
        //
        f.resize(bx,nvar);
        is.ignore(BL_IGNORE_MAX, '\n');
        switch (typ_in)
        {
        case FABio::FAB_ASCII: fio = new FABio_ascii; break;
        case FABio::FAB_8BIT:  fio = new FABio_8bit;  break;
        case FABio::FAB_NATIVE:
        case FABio::FAB_IEEE:
            fio = new FABio_binary(RealDescriptor::newRealDescriptor(typ_in,
                                                                     wrd_in,
                                                                     machine,
                                                                     FArrayBox::ordering));
            break;
        default:
            BoxLib::Error("FABio::read_header(): Unrecognized FABio header");
        }
    }
    else
    {
        //
        // The "new" FAB format.
        //
        is.putback(c);
        RealDescriptor* rd = new RealDescriptor;
        if (rd == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        is >> *rd;
        is >> bx;
        is >> nvar;
        //
        // Set the FArrayBox to the appropriate size.
        //
        f.resize(bx,nvar);
        is.ignore(BL_IGNORE_MAX, '\n');
        fio = new FABio_binary(rd);
    }

    if (is.fail())
        BoxLib::Error("FABio::read_header() failed");

    if (fio == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);

    return fio;
}

ostream&
operator<< (ostream&         os,
            const FArrayBox& f)
{
    static FABio_ascii fabio_ascii;
    fabio_ascii.write(os,f,0,f.nComp());
    return os;
}

istream&
operator>> (istream&   is,
            FArrayBox& f)
{
    FABio* fabrd = FABio::read_header(is,f);
    fabrd->read(is,f);
    delete fabrd;
    return is;
}

FArrayBox::FArrayBox (istream& ifile)
{
    readFrom(ifile);
}

void
FArrayBox::writeOn (ostream& os) const
{
    writeOn(os,0,nComp());
}

void
FArrayBox::writeOn (ostream& os,
                    int      comp,
                    int      num_comp) const
{
    boxAssert(comp >= 0 && num_comp >= 1 && (comp+num_comp) <= nComp());
    fabio->write_header(os, *this, num_comp);
    fabio->write(os, *this, comp, num_comp);
}

void
FArrayBox::readFrom (istream& is)
{
    FABio* fabrd = FABio::read_header(is, *this);
    fabrd->read(is, *this);
    delete fabrd;
}


Box
FArrayBox::skipFAB (istream& is,
                    int&     num_comp)
{
    FArrayBox f;
    FABio* fabrd = FABio::read_header(is, f);
    fabrd->skip(is, f);
    delete fabrd;
    num_comp = f.nComp();
    return f.box();
}

void
FArrayBox::skipFAB (istream& is)
{
    int ignore = 0;
    skipFAB(is, ignore);
}

void
FABio_ascii::write (ostream&         os,
                    const FArrayBox& f,
                    int              comp,
                    int              num_comp) const
{
    boxAssert(comp >= 0 && num_comp >= 1 && (comp+num_comp) <= f.nComp());

    const Box& bx = f.box();

    IntVect sm = bx.smallEnd();
    IntVect bg = bx.bigEnd();

    for (IntVect p = sm; p <= bg; bx.next(p))
    {
        os << p;
        for (int k=0; k < num_comp; k++)
            os << "  " << f(p,k+comp);
        os << "\n";
    }
    os << endl;

    if (os.fail())
        BoxLib::Error("FABio_ascii::write() failed");
}

void
FABio_ascii::read (istream&   is,
                   FArrayBox& f) const
{
    const Box& bx = f.box();

    IntVect sm = bx.smallEnd();
    IntVect bg = bx.bigEnd();
    IntVect p, q;
    for (p = sm; p <= bg; bx.next(p))
    {
        is >> q;
        boxAssert(p == q);
        for (int k = 0; k < f.nComp(); k++)
            is >> f(p, k);
    }

    if (is.fail())
        BoxLib::Error("FABio_ascii::read() failed");
}

void
FABio_ascii::skip (istream&   is,
                   FArrayBox& f) const
{
    FABio_ascii::read(is, f);
}

void
FABio_ascii::write_header (ostream&         os,
                           const FArrayBox& f,
                           int              nvar) const
{
    os << "FAB: " << FABio::FAB_ASCII << " " << 0 << " " << sys_name << '\n';
    FABio::write_header(os, f, nvar);
}

void
FABio_8bit::write (ostream&         os,
                   const FArrayBox& f,
                   int              comp,
                   int              num_comp) const
{
    boxAssert(comp >= 0 && num_comp >= 1 && (comp+num_comp) <= f.nComp());

    const Box& bx  = f.box();
    const Box& pbx = f.pBox();
    const Real eps = 1.0e-8;
    const long siz = bx.numPts();

    if (bx == pbx)
    {
        unsigned char* c = new unsigned char[siz];
        if (c == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        for (int k = 0; k < num_comp; k++)
        {
            const Real mn   = f.min(k+comp);
            const Real mx   = f.max(k+comp);
            const Real* dat = f.dataPtr(k+comp);
            Real rng = Abs(mx-mn);
            rng = (rng < eps) ? 0.0 : 255.0/(mx-mn);
            for (long i = 0; i < siz; i++)
            {
                Real v = rng*(dat[i]-mn);
                int iv = (int) v;
                c[i]   = (unsigned char) iv;
            }
            os << mn << "  " << mx << '\n' << siz << '\n';
            os.write((char*)c,siz);
        }
        delete [] c;
    }
    else
    {
        const int *lo = bx.loVect(), *plo = pbx.loVect();
        const int *hi = bx.hiVect(), *phi = pbx.hiVect();

        long off0 = lo[0] - plo[0], len0 = hi[0] - lo[0] + 1;

#if (BL_SPACEDIM==2 || BL_SPACEDIM==3)
        long plen0 = phi[0] - plo[0] + 1;
        long off1  = lo[1] - plo[1];
        long len1  = hi[1] - lo[1] + 1;
#endif

#if BL_SPACEDIM==3
        long off2  = lo[2] - plo[2];
        long plen1 = phi[1] - plo[1] + 1;
        long len2  = hi[2] - lo[2] + 1;
#endif
        unsigned char* c = new unsigned char[len0];
        if (c == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        for (int N = 0; N < num_comp; N++)
        {
            const Real mn = f.min(N+comp);
            const Real mx = f.max(N+comp);
            Real rng = Abs(mx-mn);
            rng = (rng < eps) ? 0.0 : 255.0/(mx-mn);
            os << mn << "  " << mx << '\n' << siz << '\n';
#if BL_SPACEDIM == 1
            const Real* data = f.dataPtr(N+comp) + off0;
            for (long i = 0; i < len0; i++)
            {
                Real v = rng*(data[i]-mn);
                int iv = (int) v;
                c[i]   = (unsigned char) iv;
            }
            os.write((char*)c, len0);
#elif BL_SPACEDIM == 2
            for (long j = 0; j < len1; j++)
            {
                const Real* data = f.dataPtr(N+comp)+off0+plen0*(j+off1);
                for (long i = 0; i < len0; i++)
                {
                    Real v = rng*(data[i]-mn);
                    int iv = (int) v;
                    c[i]   = (unsigned char) iv;
                }
                os.write((char*)c, len0);
            }
#else
            for (long k = 0; k < len2; ++k)
            {
                for (long j = 0; j < len1; ++j)
                {
                    const Real* data = f.dataPtr(N+comp) + off0 +
                                       plen0*(j+off1+plen1*(k+off2));
                    for (long i = 0; i < len0; i++)
                    {
                        Real v = rng*(data[i]-mn);
                        int iv = (int) v;
                        c[i]   = (unsigned char) iv;
                    }
                    os.write((char*)c, len0);
                }
            }
#endif
        }
        delete [] c;
    }

    if (os.fail())
        BoxLib::Error("FABio_8bit::write() failed");
}

void
FABio_8bit::read (istream&   is,
                  FArrayBox& f) const
{
    const Box& bx    = f.box();
    long siz         = bx.numPts();
    unsigned char* c = new unsigned char[siz];
    if (c == 0)
        BoxLib::OutOfMemory(__FILE__, __LINE__);
    Real mn, mx;
    for (int nbytes, k = 0; k < f.nComp(); k++)
    {
        is >> mn >> mx >> nbytes;
        boxAssert(nbytes == siz);
        while (is.get() != '\n')
            ;
        is.read((char*)c,siz);
        Real* dat       = f.dataPtr(k);
        const Real rng  = (mx-mn)/255.0;
        for (long i = 0; i < siz; i++)
        {
            int iv = (int) c[i];
            Real v = (Real) iv;
            dat[i] = mn + rng*v;
        }
    }
    if (is.fail())
        BoxLib::Error("FABio_8bit::read() failed");

    delete [] c;
}

void
FABio_8bit::skip (istream&   is,
                  FArrayBox& f) const
{
    const Box& bx = f.box();
    long siz      = bx.numPts();
    Real mn, mx;
    for (int nbytes, k = 0; k < f.nComp(); k++)
    {
        is >> mn >> mx >> nbytes;
        boxAssert(nbytes == siz);
        while (is.get() != '\n')
            ;
        is.seekg(siz, ios::cur);
    }

    if (is.fail())
        BoxLib::Error("FABio_8bit::skip() failed");
}

void
FABio_8bit::write_header (ostream&         os,
                          const FArrayBox& f,
                          int              nvar) const
{
    os << "FAB: " << FABio::FAB_8BIT << " " << 0 << " " << sys_name << '\n';
    FABio::write_header(os, f, nvar);
}

FABio_binary::FABio_binary (RealDescriptor* rd_)
    : rd(rd_)
{}

void
FABio_binary::write_header (ostream&         os,
                            const FArrayBox& f,
                            int              nvar) const
{
    os << "FAB " << *rd;
    FABio::write_header(os, f, nvar);
}

void
FABio_binary::read (istream&   is,
                    FArrayBox& f) const
{
    const long base_siz = f.box().numPts();
    Real* comp_ptr      = f.dataPtr(0);
    const long siz      = base_siz*f.nComp();
    RealDescriptor::convertToNativeFormat(comp_ptr, siz, is, *rd);
    if (is.fail())
        BoxLib::Error("FABio_binary::read() failed");
}

void
FABio_binary::write (ostream&         os,
                     const FArrayBox& f,
                     int              comp,
                     int              num_comp) const
{
    boxAssert(comp >= 0 && num_comp >= 1 && (comp+num_comp) <= f.nComp());

    const Box& bx  = f.box();
    const Box& pbx = f.pBox();

    if (bx == pbx)
    {
        const long base_siz  = bx.numPts();
        const Real* comp_ptr = f.dataPtr(comp);
        const long siz       = base_siz*num_comp;
        RealDescriptor::convertFromNativeFormat(os, siz, comp_ptr, *rd);
    }
    else
    {
        const int *lo = bx.loVect(), *plo = pbx.loVect();
        const int *hi = bx.hiVect(), *phi = pbx.hiVect();

        long off0 = lo[0] - plo[0], len0 = hi[0] - lo[0] + 1;

#if (BL_SPACEDIM==2 || BL_SPACEDIM==3)
        long plen0 = phi[0] - plo[0] + 1;
        long off1  = lo[1] - plo[1];
        long len1  = hi[1] - lo[1] + 1;
#endif

#if BL_SPACEDIM==3
        long off2  = lo[2] - plo[2];
        long plen1 = phi[1] - plo[1] + 1;
        long len2  = hi[2] - lo[2] + 1;
#endif

#if BL_SPACEDIM == 1
        for (int N = 0; N < num_comp; N++)
        {
            const Real* data = f.dataPtr(N+comp) + off0;
            RealDescriptor::convertFromNativeFormat(os, len0, data, *rd);
        }
#elif BL_SPACEDIM == 2
        for (int N = 0; N < num_comp; N++)
        {
            for (long j = 0; j < len1; j++)
            {
                const Real* data = f.dataPtr(N+comp)+off0+plen0*(j+off1);
                RealDescriptor::convertFromNativeFormat(os, len0, data, *rd);
            }
        }
#else
        for (int N = 0; N < num_comp; N++)
        {
            for (long k = 0; k < len2; ++k)
            {
                for (long j = 0; j < len1; ++j)
                {
                    const Real* data = f.dataPtr(N+comp) + off0 +
                                       plen0*(j+off1+plen1*(k+off2));
                    RealDescriptor::convertFromNativeFormat(os,len0,data,*rd);
                }
            }
        }
#endif
    }

    if (os.fail())
        BoxLib::Error("FABio_binary::write() failed");
}

void
FABio_binary::skip (istream&   is,
                    FArrayBox& f) const
{
    const Box& bx = f.box();
    long base_siz = bx.numPts();
    long siz      = base_siz * f.nComp();
    is.seekg(siz*rd->numBytes(), ios::cur);
    if (is.fail())
        BoxLib::Error("FABio_binary::skip() failed");
}

void
printRange (ostream&         os,
            const FArrayBox& fab,
            const Box&       reg,
            int              comp,
            int              numcomp)
{
    //
    // FIXME FIXME for aliases.
    //
    IntVect low = reg.smallEnd();
    IntVect hi  = reg.bigEnd();
    IntVect point;
    //
    // Sanity checks.
    //
    if ((! reg.ok() ) || comp < 0 || comp+numcomp > fab.nComp())
    {
        os << "printRange: bad indices:" <<  reg;
        os << " " << comp << " " << numcomp << endl;
        return;
    }
    if (!( (fab.box()).contains(reg)))
    {
        os << "printRange: indices outside FAB:" << reg << endl;
        os << fab.box() << endl;
        return;
    }

    int prec = os.precision(8);
    for (point=low; point <= hi; reg.next(point))
    {
        os << point;
        for (int k=comp; k<comp+numcomp; k++)
        {
            os << setw(15) << fab(point,k);
            if (k < comp+numcomp-1)
                if ((k-comp)%4 == 3)
                    os << "\n\t";
        }
        os << "\n";
    }
    os << setprecision(prec);
    os << endl;

    if (os.fail())
        BoxLib::Error("printRange() failed");
}

#if (BL_SPACEDIM==1)
void
printRange (ostream&         os,
            const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              comp,
            int              numcomp)
{
    IntVect low(ilo);
    IntVect hi(ihi);
    IntVect typ(fab.box().type());
    Box reg(low,hi,typ);
    printRange(os,fab,reg,comp,numcomp);
}

void
printRange (const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              comp,
            int              numcomp)
{
    printRange(cout,fab,ilo,ihi,comp,numcomp);
}

#elif (BL_SPACEDIM==2)

void
printRange (ostream&         os,
            const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              jlo,
            int              jhi,
            int              comp,
            int              numcomp)
{
    IntVect low(ilo,jlo);
    IntVect hi(ihi,jhi);
    IntVect typ(fab.box().type());
    Box reg(low,hi,typ);
    printRange(os,fab,reg,comp,numcomp);
}

void
printRange (const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              jlo,
            int              jhi,
            int              comp,
            int              numcomp)
{
    printRange(cout,fab,ilo,ihi,jlo,jhi,comp,numcomp);
}

#elif   (BL_SPACEDIM==3)
void
printRange (ostream&         os,
            const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              jlo,
            int              jhi,
            int              klo,
            int              khi,
            int              comp,
            int              numcomp)
{
    IntVect low(ilo,jlo,klo);
    IntVect hi(ihi,jhi,khi);
    IntVect typ(fab.box().type());
    Box reg(low,hi,typ);
    printRange(os,fab,reg,comp,numcomp);
}

void
printRange (const FArrayBox& fab,
            int              ilo,
            int              ihi,
            int              jlo,
            int              jhi,
            int              klo,
            int              khi,
            int              comp,
            int              numcomp)
{
    printRange(cout,fab,ilo,ihi,jlo,jhi,klo,khi,comp,numcomp);
}
#endif
