/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _INCLUDED_IRefCnt
#define _INCLUDED_IRefCnt

#include "Common/GlobalInclude.h"
#ifdef _HOST_COMPILER_MICROSOFT
#include <windows.h>
#define HAVE_ATOMIC_INC
#endif
#ifdef _HOST_COMPILER_GCC
#define HAVE_ATOMIC_INC
#endif

#ifndef HAVE_ATOMIC_INC
#error No appropriate compiler found
#endif

#ifdef _DEBUG
#ifdef _TARGET_PLATFORM_LINUX
// In debug we want to keep valgrind happy
/*#define KEEP_VALGRIND_HAPPY*/
#endif
#endif

#ifdef KEEP_VALGRIND_HAPPY
#include <valgrind/helgrind.h>
#else
#define ANNOTATE_HAPPENS_BEFORE( obj )
#define ANNOTATE_HAPPENS_AFTER( obj )

#endif 


namespace Common {
#define TO_BE_REFERRED_CONST const&
    // #define TO_BE_REFERRED_CONST 

    class IRefCnt {
    private:
        mutable volatile int m_nRefCnt;
    public:

        IRefCnt() : m_nRefCnt(0) {
        };

        inline const volatile int& getRefCnt() const {
            return m_nRefCnt;
        };

        inline int addRef() const {
#ifdef _HOST_COMPILER_MICROSOFT
            return InterlockedIncrement((LONG *) & m_nRefCnt);
#endif
#ifdef _HOST_COMPILER_GCC
            return __sync_add_and_fetch(&m_nRefCnt, 1);
#endif
        };

        inline int releaseRef() const {
            ANNOTATE_HAPPENS_BEFORE(&m_nRefCnt);
#ifdef _HOST_COMPILER_MICROSOFT
            int nRef = InterlockedDecrement((LONG *) & m_nRefCnt);
#endif
#ifdef _HOST_COMPILER_GCC
            int nRef = __sync_add_and_fetch(&m_nRefCnt, -1);
#endif
            if (nRef == 0) {
                ANNOTATE_HAPPENS_AFTER(&m_nRefCnt);
                delete this;
            }
            return nRef;
        };

    protected:

        virtual ~IRefCnt() {
        };
    };

    template <class T>
    class SmartPtrToConst {
    protected:

        union {
            const T* constPointee;
            T* pointee;
        };
    public:

        SmartPtrToConst() : constPointee(0) {
        };

        virtual ~SmartPtrToConst() {
            if (constPointee) {
                constPointee->releaseRef();
                constPointee = 0;
            }
        };

        SmartPtrToConst(const T * const& pData) : constPointee(pData) {
            if (constPointee) constPointee->addRef();
        };

        SmartPtrToConst(const SmartPtrToConst<T>& pOther) : constPointee(pOther.constPointee) {
            if (constPointee) constPointee->addRef();
        };

        SmartPtrToConst<T>& operator =(const T * const& pData) {
            if (pData) {
                pData->addRef();
            }

            if (constPointee) {
                constPointee->releaseRef();
            }

            constPointee = pData;

            return *this;
        };

        SmartPtrToConst<T>& operator =(const SmartPtrToConst<T>& pOther) {
            if (pOther.constPointee) {
                pOther.constPointee->addRef();
            }

            if (constPointee) {
                constPointee->releaseRef();
            }
            constPointee = pOther.constPointee;

            return *this;
        };

        bool operator ==(const T * const& pData) const {
            return constPointee == pData;
        }

        bool operator !=(const T * const& pData) const {
            return constPointee != pData;
        }

        bool operator<(const T * const& pData) const {
            return constPointee < pData;
        }

        operator const T* TO_BE_REFERRED_CONST() const {
            return constPointee;
        };

        const T* TO_BE_REFERRED_CONST operator -> () const {
            return constPointee;
        };

        template <class NEW_T> const
        NEW_T dyn_cast() const {
            return dynamic_cast<NEW_T> (constPointee);
        }

        template <class NEW_T> const
        NEW_T stat_cast() const {
            return static_cast<NEW_T> (constPointee);
        }

        template <class NEW_T>
        operator SmartPtrToConst<NEW_T>() const {
            return SmartPtrToConst<NEW_T > (constPointee);
        }

        const T* getRef() const {
            return constPointee;
        }

        bool operator !() const {
            return (constPointee == 0);
        };

        operator bool() const {
            return (constPointee != 0);
        };

        void attachRef(const T* newConstPointee) {
            if (constPointee) {
                SmartPtrToConst<T> other(constPointee);
                constPointee->releaseRef();
                constPointee = newConstPointee;
            } else {
                constPointee = newConstPointee;
            }
        }

        const T* detachRef() {
            const T* detached = constPointee;
            constPointee = 0;
            return detached;
        }
    };

    template <class T>
    class SmartPtr : public SmartPtrToConst<T> {
    public:

        SmartPtr() : SmartPtrToConst<T>() {
        }

        virtual ~SmartPtr() {
            if (this->pointee) {
                this->pointee->releaseRef();
                this->pointee = 0;
            }
        };

        SmartPtr(T * const& pData) : SmartPtrToConst<T>() {
            if (pData) {
                pData->addRef();
                this->pointee = pData;
            }
        };

        SmartPtr(const SmartPtr<T>& pOther) : SmartPtrToConst<T>() {
            if (pOther.pointee) {
                pOther.pointee->addRef();
            }
            this->pointee = pOther.pointee;
        };

        SmartPtr<T>& operator =(T * const& pData) {
            if (pData) {
                pData->addRef();
            }

            if (this->pointee) {
                this->pointee->releaseRef();
            }

            this->pointee = pData;

            return *this;
        };

        SmartPtr<T>& operator =(const SmartPtr<T>& pOther) {
            if (pOther.pointee) {
                pOther.pointee->addRef();
            }

            if (this->pointee) {
                this->pointee->releaseRef();
            }

            this->pointee = pOther.pointee;

            return *this;
        };

        operator T* TO_BE_REFERRED_CONST() const {
            return this->pointee;
        };

        T* TO_BE_REFERRED_CONST operator -> () const {
            return this->pointee;
        };

        template <class NEW_T>
        NEW_T dyn_cast() const {
            return dynamic_cast<NEW_T> (this->pointee);
        }

        template <class NEW_T> const
        NEW_T stat_cast() const {
            return static_cast<NEW_T> (this->pointee);
        }

        template <class NEW_T>
        operator SmartPtr<NEW_T>() const {
            return SmartPtr<NEW_T > (this->pointee);
        }

        T* getRef() const {
            return this->pointee;
        }

        void attachRef(T* newPointee) {
            if (this->pointee) {
                SmartPtr<T> other(this->pointee);
                this->pointee->releaseRef();
                this->pointee = newPointee;
            } else {
                this->pointee = newPointee;
            }
        }

        T* detachRef() {
            T* detached = this->pointee;
            this->pointee = 0;
            return detached;
        }

        bool operator !() const {
            return (this->pointee == 0);
        };

        operator bool() const {
            return (this->pointee != 0);
        };
    };
}


#endif // #ifndef _INCLUDED_IRefCnt
