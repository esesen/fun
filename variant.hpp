#ifndef VARIANT_HPP
# define VARIANT_HPP

#include <stdexcept>
#include "type_list.hpp" 



// variant
template<class... Types> 
class variant;

template<>
class variant<> {
  template<class... Ts>
  friend class variant;

  void destroy_() {
    // TODO: throw
  }

public:
  template<class Type>
  variant(Type &&) = delete;

}; 


template<class Type, class... Types>
class variant<Type, Types...> {
  template<class... Ts>
  friend class variant;

  template<class T>
  using type_tag = typename type_list<Type, Types...>::reverse::template position<T>;

  union {
    struct {
      size_t tag;
      Type value;
    };

    variant<Types...> other;
  };

  void destroy_() {
    if (type_tag<Type>::value == tag)
      value.~Type();
    else other.destroy_();
  }

public:  
  // ctors / dtors
  variant(Type &&a)
      : tag(type_tag<Type>::value)
      , value(std::forward<Type> (a)) {}

  template<class T>
  variant(T &&a)
    : other(std::forward<T>(a)) {}

  ~variant() {
    destroy_();
  }

  variant & operator=(Type a) {
    tag = type_tag<Type>::value;
    new (&value) Type(std::move(a));

    return *this;
  }
  
  template<class T>
  variant & operator=(T &&a) {
    destroy_();
    other = std::forward<T> (a);
    return *this;
  }
};



#endif /* VARIANT_HPP */
