#ifndef TYPE_LIST_HPP
# define TYPE_LIST_HPP


#include <type_traits>



namespace type_list_ {

  // position of a type in a type list
  template<class Element, class Head, class... Tail>
  struct position {
    static constexpr size_t value = 1 + position<Element, Tail...>::value;
  };

  template<class Element, class... Tail>
  struct position<Element, Element, Tail...> {
    static constexpr size_t value = 0;
  };

  
  // foldl
  template<class Init, template<class,class> class Op, class... List>
  struct foldl;

  template<class Init, template<class,class> class Op, class Head, class... Tail>
  struct foldl<Init, Op, Head, Tail...> {
    using type = typename foldl<typename Op<Init, Head>::type, Op, Tail...>::type; 
  };

  template<class Init, template<class,class> class Op>
  struct foldl<Init, Op> {
    using type = Init;
  };

} /* namespace type_list_ */


// is_elem
template<class Element, class... List>
struct is_elem;

template<class Element, class Head, class... Tail>
struct is_elem<Element, Head, Tail...> {
  using type = typename is_elem<Element, Tail...>::type;
};

template<class Element, class... Tail>
struct is_elem<Element, Element, Tail...> {
  using type = std::true_type;
};

template<class Element>
struct is_elem<Element> {
  using type = std::false_type;
};
 

// both_true -- &&
template<class, class>
struct both_true;

template<>
struct both_true<std::true_type, std::true_type>
{
  using type = std::true_type;
};

template<bool A, bool B>
struct both_true<
    std::integral_constant<bool, A>
  , std::integral_constant<bool, B>>
{
  using type = std::false_type;
};



// either_true -- ||
template<class, class>
struct either_true;

template<>
struct either_true<
    std::false_type
  , std::false_type>
{
  using type = std::false_type;
};

template<bool A, bool B>
struct either_true<
    std::integral_constant<bool, A>
  , std::integral_constant<bool, B>>
{
  using type = std::true_type;
};



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

  template<template<class> class F>
  using map = type_list<F<Head>, F<Tail>...>;

  template<template<class,class> class Op>
  using fold = typename type_list_::foldl<Head, Op, Tail...>::type;

  template<class T>
  using has_elem = typename is_elem<T, Head, Tail...>::type;

  template<class... Ts>
  using is_superset = typename type_list<Ts...>::template map<has_elem>::template fold<both_true>::type;
};



template<class C>
struct size_of
{
  static constexpr size_t value = sizeof(C);
};

template<class C>
struct align_of
{
  static constexpr size_t value = alignof(C);
};



template<class C, class D>
struct greater_of
{
  using type = typename std::conditional<(C::value < D::value)
      , D
      , C>::type;
};



template<class T, T N, T M>
struct greater_of<std::integral_constant<T,N>, std::integral_constant<T,M>>
{
  using type = typename std::conditional<(N < M)
      , std::integral_constant<T,M>
      , std::integral_constant<T,N>
    >::type;
};



#endif /* TYPE_LIST_HPP */
