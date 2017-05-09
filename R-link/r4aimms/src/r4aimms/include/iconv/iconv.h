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
#ifndef _LIBICONV_H
#define _LIBICONV_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
    typedef void* iconv_t;
    iconv_t iconv_open(const char *tocode, const char *fromcode);
    int iconv_close(iconv_t cd);
    size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
#ifdef __cplusplus
}
#endif
#endif//_LIBICONV_H