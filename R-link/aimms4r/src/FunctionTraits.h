#pragma once
#include <functional>
#include <iostream>
#include <cassert>

namespace Aimms{

template<typename T>
struct FunctionTraits;

template<typename R, typename ...Args>
struct FunctionTraits<std::function<R(Args...)>>
{
  static const size_t nargs = sizeof...(Args);

  typedef R result_type;

  template <size_t i>
  struct arg
  {
    typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
  };

  template<std::size_t I = 0, typename... Tp>
  static inline typename std::enable_if<I == sizeof...(Tp), int>::type
    getSize(std::tuple<Tp...>& t, int* size)
    { return 0;}

  template<std::size_t I = 0, typename... Tp>
  static inline typename std::enable_if<I < sizeof...(Tp), int>::type
    getSize(std::tuple<Tp...>& t, int* size)
    {
      *size +=  sizeof( std::get<I>(t) );
      return getSize<I + 1, Tp...>(t,size);
    }

  static int  calculateSize(void){
    std::tuple<Args...> ttt;
    int size = 0;
    (void)getSize(ttt,&size);
    return size;
  }

  static void printArgs(void){
    std::tuple<Args...> ttt;
    print(ttt);
  }

  template <std::size_t I = 0, typename... Tp>
  static inline typename std::enable_if<I == sizeof...(Tp), void>::type
    print(std::tuple<Tp...>& t)
    { }

  template<std::size_t I = 0, typename... Tp>
  static inline typename std::enable_if<I < sizeof...(Tp), void>::type
    print(std::tuple<Tp...>& t)
    {
      std::cout << std::get<I>(t) << std::endl;
      print<I + 1, Tp...>(t);
    }

};
}
