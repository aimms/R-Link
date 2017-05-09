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
#ifndef _INCLUDED__Common_ExportSTL
#define _INCLUDED__Common_ExportSTL

#include "Common/GlobalInclude.h"

#if _MSC_VER < 1600
#define EXPORT_STL_STRING( declspec, char_t ) 

#define EXPORT_STL_VECTOR( declspec, vectype )

#else
// this is to get rid of warning C4521,
// see also http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

#define EXPORT_STL_STRING( declspec, char_t ) \
    template class declspec std::allocator<char_t>; \
    template class declspec std::basic_string<char_t, std::char_traits<char_t>, std::allocator<char_t> >; 

#define EXPORT_STL_VECTOR( declspec, vectype ) \
    template class declspec std::allocator< vectype >; \
    template class declspec std::vector<vectype, std::allocator< vectype > >;

#endif

#endif  // _INCLUDED__Common_ExportSTL
