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
