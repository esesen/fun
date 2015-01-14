#ifndef VARIADIC_HPP
# define VARIADIC_HPP



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



#endif /* VARIADIC_HPP */
