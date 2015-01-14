#ifndef TYPE_LIST_HPP
# define VARIADIC_HPP

namespace type_list_ {

// position of a type in a type list
template<class Element, class Head, class... Tail>
struct position
{
  static constexpr size_t value = 1 + position<Element, Tail...>::value;
};


template<class Element, class... Tail>
struct position<Element, Element, Tail...>
{
  static constexpr size_t value = 0;
};

} /* namespace type_list_ */


// type_list
template<class... Types>
struct type_list;


template<>
struct type_list<>
{
  template<template<class...> class C>
  using instantiate = C<>;
  using reverse = type_list;

  template<class... Ts>
  using append = type_list<Ts...>;
};


template<class Head, class... Tail>
struct type_list<Head, Tail...>
{
  using head = Head;
  using tail = type_list<Tail...>;

  template<template<class...> class C>
  using instantiate = C<Head, Tail...>;

  template<class T>
  using position = type_list_::position<T, Head, Tail...>;

  template<class... Ts>
  using append = type_list<Head, Tail..., Ts...>;

  using reverse = typename tail::reverse::template append<head>::template instantiate<type_list>;
};




#endif /* VARIADIC_HPP */
