#ifndef VARIANT_HPP
# define VARIANT_HPP

#include <stdexcept>
#include "type_list.hpp" 

#include <typeinfo>



struct boxed_type
{
  virtual ~boxed_type() {};
};

template<class T>
struct boxed
  : public boxed_type
{
  T value;

  template<class... Args>
  boxed(Args&&... args)
    : value(std::forward<Args> (args)...)
  {}

  ~boxed() {}
};


template<class... Types>
class variant
{
  static constexpr size_t max_value_size
      = type_list<boxed<Types>...>::template map<size_of>::template fold<greater_of>::value;

  static constexpr size_t strictest_value_alignment
      = type_list<boxed<Types>...>::template map<align_of>::template fold<greater_of>::value;

  alignas(strictest_value_alignment) char store_[max_value_size];

  boxed_type const * ptr_() const {
    return reinterpret_cast<boxed_type const *> (&store_);
  }

  boxed_type * ptr_() {
    return reinterpret_cast<boxed_type *> (&store_);
  }

  template<class T>
  boxed<T> const & ref_() const {
    auto ptr = dynamic_cast<boxed<T> const *>(ptr_());
    if (! ptr)
      throw std::bad_cast();

    return *ptr;
  }

  template<class T>
  boxed<T> & ref_() {
    auto ptr = dynamic_cast<boxed<T> *>(ptr_());
    if (! ptr)
      throw std::bad_cast();

    return *ptr;
  }


public:
  // constructors / destructors
  template<
      class T
    , class = typename std::enable_if<
        type_list<Types...>::template has_elem<T>::value>::type>
  variant(T &&a) {
    new (ptr_()) boxed<T> (std::forward<T> (a));
  }

  ~variant() { ptr_()->~boxed_type(); };


  // accessors
  template<class T>
  typename std::enable_if<
      type_list<Types...>::template has_elem<T>::value
    , T &>::type
  get() {
    return ref_<T>().value;
  }

  template<class T>
  typename std::enable_if<
      type_list<Types...>::template has_elem<T>::value
    , T const &>::type
  get() const {
    return ref_<T>().value;
  }


  // mutators
  template<class... Ts>
  typename std::enable_if<
      type_list<Types...>::template is_superset<Ts...>::type::value
    , variant &>::type
  operator=(variant<Ts...> const &a) {
    return *this;
  }

  template<class T>
  typename std::enable_if<
      type_list<Types...>::template has_elem<T>::value
    , variant &>::type
  operator=(T &&a) {
    ptr_()->~boxed_type();
    new (ptr_()) boxed<T>(std::forward<T> (a));

    return *this;
  }
};


#endif /* VARIANT_HPP */
