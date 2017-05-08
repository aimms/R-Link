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
