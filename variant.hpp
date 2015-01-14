#ifndef VARIANT_HPP
# define VARIANT_HPP

#include <stdexcept>
#include "variadic.hpp" 



// variant
template<class... Types> 
class variant;

template<>
class variant<> {
public:
  template<class Type>
  variant(Type &&) = delete;
}; 


template<class Type, class... Types>
class variant<Type, Types...> {

  template<class SomeType>
  struct type_tag {
    using type = SomeType;
    static constexpr size_t value = sizeof...(Types) - position<SomeType, Type, Types...>::value;
  };

  union {
    struct {
      size_t tag;
      Type value;
    };

    variant<Types...> other;
  };


public:  
  // ctors / dtors
  template<class SomeType>
  variant(SomeType &&a)
    : other(std::forward<SomeType>(a)) {}

  variant(Type &&a)
    : tag(type_tag<Type>::value), value(std::forward<Type> (a)) {}

  ~variant() {
    if (type_tag<Type>::value == tag)
      value.~Type();
    else other.~variant<Types...>();
  }
};



#endif /* VARIANT_HPP */
