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
HRESULT WINAPI ConvertINetString(
    LPDWORD lpdwMode,
    DWORD dwSrcEncoding,
    DWORD dwDstEncoding,
    LPCSTR lpSrcStr,
    LPINT lpnSrcSize,
    LPBYTE lpDstStr,
    LPINT lpnDstSize
);

HRESULT WINAPI ConvertINetMultiByteToUnicode(
    LPDWORD lpdwMode,
    DWORD dwSrcEncoding,
    LPCSTR lpSrcStr,
    LPINT lpnMultiCharCount,
    LPWSTR lpDstStr,
    LPINT lpnWideCharCount
);

HRESULT WINAPI ConvertINetUnicodeToMultiByte(
    LPDWORD lpdwMode,
    DWORD dwEncoding,
    LPCWSTR lpSrcStr,
    LPINT lpnWideCharCount,
    LPSTR lpDstStr,
    LPINT lpnMultiCharCount
);

HRESULT WINAPI IsConvertINetStringAvailable(
    DWORD dwSrcEncoding,
    DWORD dwDstEncoding
);

HRESULT WINAPI LcidToRfc1766A(
    LCID Locale,
    LPSTR pszRfc1766,
    int nChar
);

HRESULT WINAPI LcidToRfc1766W(
    LCID Locale,
    LPWSTR pszRfc1766,
    int nChar
);

HRESULT WINAPI Rfc1766ToLcidA(
    LCID *pLocale,
    LPSTR pszRfc1766
);

HRESULT WINAPI Rfc1766ToLcidW(
    LCID *pLocale,
    LPWSTR pszRfc1766
);
