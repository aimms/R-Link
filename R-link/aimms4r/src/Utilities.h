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
#pragma once

inline std::wstring a2w(std::string mbString){
  std::wstring wideString;
  if (mbString.length() > 0){
    size_t nLenRequired = mbstowcs(0, mbString.c_str(), 0);
    if (nLenRequired == ((size_t)-1)){
      throw std::runtime_error("mbstowcs: conversion failed");
    }
    wideString.resize(nLenRequired);
    size_t result = mbstowcs(&wideString[0], mbString.c_str(), nLenRequired + 1);
    if (result != nLenRequired){
      throw std::runtime_error("mbstowcs: conversion failed");
    }
  }
  return wideString;
}

inline std::string w2a(std::wstring wideString){
  std::string mbsString;
  if (wideString.length() > 0){
    size_t nLenRequired = wcstombs(0, wideString.c_str(), 0);
    if (nLenRequired == ((size_t)-1)){
      throw std::runtime_error("wcstombs: conversion failed");
    }
    mbsString.resize(nLenRequired);
    size_t result = wcstombs(&mbsString[0], wideString.c_str(), nLenRequired + 1);
    if (result != nLenRequired){
      throw std::runtime_error("wcstombs: conversion failed");
    }
  }
  return mbsString;
}
