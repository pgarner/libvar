/*
 * Copyright 2013 by Philip N. Garner
 *
 * See the file COPYING for the licence associated with this software.
 *
 * Author(s):
 *   Phil Garner, July 2013
 */

#ifndef VAR_H
#define VAR_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <complex>
#include <chrono>

#include <ind.h>

namespace libvar
{
    typedef std::complex<float> cfloat;
    typedef std::complex<double> cdouble;

    class varheap;
    class var;

    /**
     * The possible var types
     */
    enum {
        TYPE_ARRAY = 0,
        TYPE_CHAR,
        TYPE_INT,
        TYPE_LONG,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_CFLOAT,
        TYPE_CDOUBLE,
        TYPE_VAR,
        TYPE_PAIR
    };


    /**
     * Unary functor
     *
     * A unary functor just acts on itself.
     */
    class UnaryFunctor
    {
    public:
        UnaryFunctor() { mDim = 0; };
        virtual ~UnaryFunctor() {};
        var operator ()(const var& iVar) const;
        var operator ()(const var& iVar, var& oVar) const;
    protected:
        int mDim;
        virtual void broadcast(var iVar, var& oVar) const;
        virtual var alloc(var iVar) const;
        virtual void scalar(const var& iVar, var& oVar) const;
        virtual void vector(
            var iVar, ind iOffsetI, var& oVar, ind iOffsetO
        ) const;
        virtual void vector(var iVar, var& oVar) const;
    };


    /**
     * Binary functor
     *
     * A binary functor is a functor of two variables.
     */
    class BinaryFunctor
    {
    public:
        virtual ~BinaryFunctor() {};
        var operator ()(
            const var& iVar1, const var& iVar2
        ) const;
        var operator ()(
            const var& iVar1, const var& iVar2, var& oVar
        ) const;
    protected:
        virtual void broadcast(var iVar1, var iVar2, var& oVar) const;
        virtual var alloc(var iVar) const;
        virtual void scalar(
            const var& iVar1, const var& iVar2, var& oVar
        ) const;
        virtual void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
    };


#   define BASIC_UNARY_FUNCTOR_DECL(f)                      \
    class f : public UnaryFunctor                           \
    {                                                       \
    protected:                                              \
        void scalar(const var& iVar, var& oVar) const;      \
    };

#   define BASIC_BINARY_FUNCTOR_DECL(f)                     \
    class f : public BinaryFunctor                          \
    {                                                       \
    protected:                                              \
        void scalar(                                        \
            const var& iVar1, const var& iVar2, var& oVar   \
        ) const;                                            \
    };

    BASIC_UNARY_FUNCTOR_DECL(NormC)
    BASIC_UNARY_FUNCTOR_DECL(Sin)
    BASIC_UNARY_FUNCTOR_DECL(Cos)
    BASIC_UNARY_FUNCTOR_DECL(Tan)
    BASIC_UNARY_FUNCTOR_DECL(Floor)
    BASIC_UNARY_FUNCTOR_DECL(Sqrt)
    BASIC_UNARY_FUNCTOR_DECL(Log)
    BASIC_UNARY_FUNCTOR_DECL(Exp)
    BASIC_BINARY_FUNCTOR_DECL(Pow)


    template <class T>
    BASIC_UNARY_FUNCTOR_DECL(Cast)


    /**
     * Set/Copy functor
     */
    class Set : public BinaryFunctor
    {
    protected:
        void scalar(const var& iVar1, const var& iVar2, var& oVar) const;
        void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
    };


    /**
     * Addition functor
     */
    class Add : public BinaryFunctor
    {
    protected:
        void scalar(const var& iVar1, const var& iVar2, var& oVar) const;
        void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
    };


    /**
     * Subtraction functor
     */
    class Sub : public BinaryFunctor
    {
    protected:
        void scalar(const var& iVar1, const var& iVar2, var& oVar) const;
        void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
    };


    /**
     * Multiplication functor
     */
    class Mul : public BinaryFunctor
    {
    protected:
        void broadcast(var iVar1, var iVar2, var& oVar) const;
        void scalar(const var& iVar1, const var& iVar2, var& oVar) const;
        void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
        void scale(var iVar1, var iVar2, var& oVar, int iOffset) const;
    };


    /**
     * Dot product functor
     */
    class Dot : public BinaryFunctor
    {
    protected:
        var alloc(var iVar) const;
        void vector(
            var iVar1, ind iOffset1, var iVar2, var& oVar, ind iOffsetO
        ) const;
    };


    /**
     * Division functor
     * There's no array operation.
     */
    class Div : public BinaryFunctor
    {
    protected:
        void scalar(const var& iVar1, const var& iVar2, var& oVar) const;
    };


    /**
     * Absolute value functor
     */
    class Abs : public UnaryFunctor
    {
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
    };


    /**
     * Norm functor
     */
    class Norm : public UnaryFunctor
    {
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
    };


    /**
     * Absolute sum functor
     */
    class ASum : public UnaryFunctor
    {
    public:
        ASum() { mDim = 1; };
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
        void vector(var iVar, ind iOffsetI, var& oVar, ind iOffsetO) const;
    };


    /**
     * Basic sum functor
     */
    class Sum : public UnaryFunctor
    {
    public:
        Sum() { mDim = 1; };
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
        void vector(var iVar, ind iOffsetI, var& oVar, ind iOffsetO) const;
    };


    /**
     * Transpose functor
     */
    class Transpose : public UnaryFunctor
    {
    public:
        Transpose() { mDim = 2; };
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
        void vector(var iVar, ind iOffsetI, var& oVar, ind iOffsetO) const;
    };


    /**
     * Index of absolute maximum value functor
     */
    class IAMax : public UnaryFunctor
    {
    public:
        IAMax() { mDim = 1; };
    protected:
        var alloc(var iVar) const;
        void vector(
            var iVar, ind iOffsetI, var& oVar, ind iOffsetO
        ) const;
    };


    // Statics
    extern const var nil;

    // Casting functors
    extern Cast<char> castChar;
    extern Cast<int> castInt;
    extern Cast<long> castLong;
    extern Cast<float> castFloat;
    extern Cast<double> castDouble;
    extern Cast<cfloat> castCFloat;
    extern Cast<cdouble> castCDouble;

    // stdlib Functors
    extern Abs abs;
    extern Norm norm;
    extern NormC normc;
    extern Sin sin;
    extern Cos cos;
    extern Tan tan;
    extern Floor floor;
    extern Sqrt sqrt;
    extern Log log;
    extern Exp exp;
    extern Pow pow;

    // BLAS functors
    extern Set set;
    extern Add add;
    extern Sub sub;
    extern Mul mul;
    extern Dot dot;
    extern Div div;
    extern ASum asum;
    extern Sum sum;
    extern IAMax iamax;

    // BLAS-like
    extern Transpose transpose;

    /**
     * Class with runtime type determination.
     *
     * The name 'var' is borrowed from ECMAScript.  The syntax is intended to
     * be more from ruby, but we can't use delete() method!() or method?().
     */
    class var
    {
    public:

        // Special member functions
        var();
        ~var();
        var(const var& iVar);
        var& operator =(var iVar);
        var(var&& iVar);

        // Overloaded constructors
        var(char iData);
        var(int iData);
        var(long iData);
        var(float iData);
        var(double iData);
        var(cfloat iData);
        var(cdouble iData);
        var(const char* iData);
        var(int iSize, const char* iData);
        var(int iSize, const char* const* iData);
        var(const char* const* iData);
        var(int iSize, const int* iData);
        var(int iSize, var iVar);

        // Data accessor
        template<class T> T get() const;
        template<class T> T* ptr(ind iIndex=0);
        const char* str();

        // Operators
        bool operator !=(var iVar) const;
        bool operator <(var iVar) const;
        bool operator ==(var iVar) const { return !(*this != iVar); };
        bool operator >(var iVar) const { return iVar < *this; };
        bool operator <=(var iVar) const { return !(*this > iVar); };
        bool operator >=(var iVar) const { return !(*this < iVar); };
        var& operator +=(var iVar) { add(*this, iVar, *this); return *this; };
        var& operator -=(var iVar) { sub(*this, iVar, *this); return *this; };
        var& operator *=(var iVar) { mul(*this, iVar, *this); return *this; };
        var& operator /=(var iVar) { div(*this, iVar, *this); return *this; };
        var operator +(var iVar) const { return add(*this, iVar); };
        var operator -(var iVar) const { return sub(*this, iVar); };
        var operator *(var iVar) const { return mul(*this, iVar); };
        var operator /(var iVar) const { return div(*this, iVar); };
        var operator [](int iIndex);
        var operator [](int iIndex) const { return at(iIndex); };
        var operator [](var iVar);
        var operator ()(int iFirst, ...) const;
        explicit operator bool() const { return defined(); };
        var operator ,(var iVar) { return push(iVar); };

        // Methods
        var& dereference();
        template<class T> T cast() const;
        bool is(var& iVar) const;
        var at(int iIndex) const;
        var at(var iVar) const;
        var key(int iIndex) const;
        var copy(bool iAllocOnly=false) const;
        bool defined() const;
        int size() const;
        ind type() const;
        ind atype() const;
        var typeStr() const;
        var atypeStr() const;
        varheap* heap() const;
        var pop();
        var top() { return at(size() - 1); };
        var& push(var iVar);
        var& insert(var iVar, int iIndex=0);
        var remove(int iIndex);
        var& unshift(var iVar) { return insert(iVar); };
        var& append(var iVar) { return insert(iVar, size()); };
        var shift() { return remove(0); };
        var sort() const;
        ind index(var iVar) const;
        var& clear();
        var& array();
        var& resize(int iSize);
        var& presize(int iSize);
        void format(std::ostream& iStream, int iIndent = 0) const;

        // Methods for functors in math.cpp
        var abs() { return libvar::abs(*this, *this); };
        var norm() { return libvar::normc(*this, *this); };
        var floor() { return libvar::floor(*this, *this); };
        var sin() { return libvar::sin(*this, *this); };
        var cos() { return libvar::cos(*this, *this); };
        var sqrt() { return libvar::sqrt(*this, *this); };
        var log() { return libvar::log(*this, *this); };
        var exp() { return libvar::exp(*this, *this); };
        var pow(var iPow) { return libvar::pow(*this, iPow, *this); };

        // Other functors
        var transpose() { return libvar::transpose(*this, *this); };

        // string.cpp
        var& getline(std::istream& iStream);
        var split(const char* iStr, int iMax=0) const;
        var join(const char* iStr) const;
        var strip();
        var& sprintf(const char* iFormat, ...);
        bool search(var iRE);
        bool match(var iRE);
        var replace(var iRE, var iStr);
        var& toupper();
        var& tolower();

        // Files
        var& read(const char* iFile, const char* iType);
        var& write(const char* iFile, const char* iType);

        // Tensors
        bool view() const;
        var view(const std::initializer_list<int> iList, int iOffset=0);
        var view(var iShape, int iOffset=0);
        var subview(int iDim, ind iOffset);
        int offset() const;
        var& offset(int iOffset);
        var& advance(int iAdvance) { return offset(offset() + iAdvance); };
        var shape() const;
        int shape(int iDim) const;
        int stride(int iDim) const;
        void bounds(int iDim, int iIndex) const;
        int dim() const;

    private:

        union dataType {
            dataType() { hp = 0; }; // Because of cfloat
            char c;
            int i;
            long l;
            float f;
            double d;
            cfloat cf;
            varheap* hp;
        };

        // Putting the data first makes sure it will be aligned on an
        // allocation boundary
        dataType mData; ///< The payload
        ind mType;      ///< The data type or reference index

        // Methods
        var& varderef();
        bool reference() const;
        var reference(int iIndex) const;
        int attach(varheap* iHeap=0);
        int detach(varheap* iHeap=0);
        int binary(var iData) const;
        void setStrides(var& iVar, int iSize, int iOffset);
    };


    /*
     * Functions
     */
    std::ostream& operator <<(std::ostream& iStream, var iVar);
    var view(const std::initializer_list<int> iShape, var iType=nil);
    var view(var iShape, var iType=nil);
    const char* typeStr(ind iType);
    var range(var iLo, var iHi, var iStep=1);
    var range(var iHi);
    var irange(var iLo, var iHi, var iStep=1);
    var irange(var iHi);


    /**
     * A stream buffer that uses a var as the buffer
     */
    class varbuf : public std::streambuf
    {
    public:
        varbuf(var iVar);
        explicit operator var () const { return mVar; };
    protected:
        virtual int_type overflow(int_type iInt = traits_type::eof());
        virtual int_type uflow();
        virtual int_type underflow();
    private:
        var mVar; ///< The var being accessed
    };


    /**
     * An ostream that uses a var as the buffer
     */
    class vstream : public std::iostream
    {
    public:
        vstream(var iVar = nil);
        explicit operator var() const { return var(mVarBuf); };
        const char* str() { return var(mVarBuf).str(); };
    private:
        class varbuf mVarBuf;
    };


    /**
     * Exception class
     */
    class vruntime_error : public std::exception
    {
    public:
        vruntime_error(var iVar);
        virtual const char* what() const noexcept;
    private:
        var mVar;
        char* mStr;
    };


    /**
     * Base class for modules (dynamically loaded classes)
     */
    class Module
    {
    public:
        virtual ~Module() {};
     };


    extern "C" {
        /**
         * Function with C linkage that must exist in the dynamically loaded
         * library.  It should return a varfile by calling new on the derived
         * class within the library.  It is part of the interface.
         */
        void factory(Module** oModule);
    }


    /**
     * Module factory
     *
     * This is actually a loader for the module rather than the module itself.
     * The create() method is a factory method that generates modules.
     */
    class module
    {
    public:
        module(const char* iType);
        virtual ~module();
        Module* create() const;
    protected:
        Module* mInstance; ///< Pointer to instance of module
    private:
        void* mHandle;     ///< Handle for dynamic library
    };


    /**
     * Abstract class for dynamically loaded file handlers.  Defines the
     * interface that file handlers must implement.
     */
    class varfile : public Module
    {
    public:
        virtual ~varfile() {};
        virtual var read(const char* iFile) = 0;
        virtual void write(const char* iFile, var iVar) = 0;
    };


    /**
     * Class to dynamically load a file handler
     */
    class vfile : public module
    {
    public:
        vfile(const char* iType="txt") : module(iType) {};
        var read(const char* iFile) {
            return dynamic_cast<varfile*>(mInstance)->read(iFile);
        };
        void write(const char* iFile, var iVar) {
            return dynamic_cast<varfile*>(mInstance)->write(iFile, iVar);
        };
    };


    struct DFTImpl;
    /**
     * DFT functor
     *
     * Uses the "pimpl" pattern: pointer to implementation.  This means the
     * implementation can be dependent upon whichever library is available at
     * compile time.
     */
    class DFT : public UnaryFunctor
    {
    public:
        DFT(int iSize, bool iInverse=false, var iForwardType=0.0f);
        ~DFT();
    protected:
        var alloc(var iVar) const;
        void scalar(const var& iVar, var& oVar) const;
        void vector(var iVar, ind iOffsetI, var& oVar, ind iOffsetO) const;
    private:
        DFTImpl* mImpl;
    };


    /**
     * Timer class
     */
    class timer
    {
    public:
        timer(var iName);
        ~timer();
    private:
        var mName;
        std::chrono::steady_clock::time_point mBegan;
    };

}

#endif // VAR_H
