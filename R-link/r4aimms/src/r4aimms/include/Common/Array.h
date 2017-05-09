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
