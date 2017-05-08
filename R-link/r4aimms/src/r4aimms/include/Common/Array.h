#ifndef _INCLUDED__Common_Array
#define _INCLUDED__Common_Array

namespace Common {

    template <typename T>
    class Array {
    private:
        size_t m_Size;
        T* m_arrayData;
    public:
		typedef const T * const_iterator;
		typedef T * iterator;
    public:

        Array(size_t theSize) : m_Size(theSize) , m_arrayData(new T[theSize]) {}
        ~Array() { delete [] m_arrayData; }

		size_t size() const { return m_Size;}

        // stl like iterators
		const_iterator begin() const    { return m_arrayData; }
		const_iterator end() const      { return m_arrayData+m_Size; }

		iterator begin()        { return m_arrayData; }
		iterator end()          { return m_arrayData+m_Size; }

        // array acccess
        T* data()               { return m_arrayData;}
        const T* data() const   { return m_arrayData;}

        // elment acccess
        T& operator[] (int index)               {return m_arrayData[index];}
        const T& operator[] (int index) const   {return m_arrayData[index];}
    };
};

#endif // end #ifndef _INCLUDED__Common_Array
